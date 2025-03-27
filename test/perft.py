from subprocess import Popen, PIPE
import chess

Engine_1_path = "../../Chess-Analysis/backend/logic/Stockfish/src/stockfish"
Engine_2_path = "../nchess"


def send_command(process: Popen, command: str) -> str:
    process.stdin.write(f"{command}\n".encode())
    process.stdin.flush()

    buf = ""

    while True:
        output = process.stdout.readline().decode()
        if output:
            buf += output
            if "Nodes searched:" in output:
                return buf
            
def parse_output(output: str) -> set:
    output = output.split("\n")
    output = [line for line in output if ((": ") in line) and (("info") not in line) and ("Nodes" not in line)]
    # Nodes will always be first, then the rest will be moves
    output = set(sorted(set(output)))
    return output

def close_engine(Engine: Popen) -> None:
    Engine.stdin.write("quit\n".encode())
    Engine.stdin.flush()

def find_inaccuracies(engine_1: Popen, engine_2: Popen, board: chess.Board, depth: int, wrong: set) -> list:
    inaccuracies = []
    for mv in wrong:
        move = mv.split(":")[0]
        board.push_uci(move)
        fen = board.fen()
        engine_1_output = send_command(engine_1, f"position fen {fen}\ngo perft {depth}")
        engine_2_output = send_command(engine_2, f"position fen {fen}\ngo perft {depth}")
        engine_1_output = parse_output(engine_1_output)
        engine_2_output = parse_output(engine_2_output)
        wrong = engine_1_output - engine_2_output
        if depth == 1:
            inaccuracies.append((fen, engine_2_output, wrong, len(engine_1_output)))
        else:
            inaccuracies += find_inaccuracies(engine_1, engine_2, board, depth-1, wrong)

        board.pop()

    return inaccuracies
        



def compare_engines(engine_1: Popen, engine_2: Popen, test_suite: list[tuple]) -> list:
    inaccuracies = []
    for position in test_suite:
        # we technically do not need the node count here because the reference engine already has it
        fen, depth, _ = position
        print(f"{fen} {depth}")
        engine_1_output = send_command(engine_1, f"position fen {fen}\ngo perft {depth}")
        engine_2_output = send_command(engine_2, f"position fen {fen}\ngo perft {depth}")
        engine_1_output = parse_output(engine_1_output)
        engine_2_output = parse_output(engine_2_output)
        wrong = engine_1_output - engine_2_output
        if wrong:
            if depth == 1:
                inaccuracies.append((fen, engine_2_output, wrong, len(engine_1_output)))

            else:
                board = chess.Board(fen)
                inaccuracies += find_inaccuracies(engine_1, engine_2, board, depth-1, wrong)


    return inaccuracies


if __name__ == "__main__":
    # reference engine
    Engine_1 = Popen([f'./{Engine_1_path}'], stdout=PIPE, stdin=PIPE)
    # your engine
    Engine_2 = Popen([f'./{Engine_2_path}'], stdout=PIPE, stdin=PIPE)
    with open("test_suite.txt", "r") as fh:
        test_suite = fh.read()
        test_suite = test_suite.split("\n")
        test_suite = [tuple(line.split(";")) for line in test_suite if not line.startswith("#")]
        test_suite = [(fen, int(depth), int(nodes)) for fen, depth, nodes in test_suite]

    # print(test_suite)
    inaccuracies = compare_engines(Engine_1, Engine_2, test_suite)
    print("started writing")
    with open("inaccuracies.txt", "w+") as fh:
        for inaccuracy in inaccuracies:
            fen, predicted, wrong_moves, correct_nodes = inaccuracy
            predicted = " ".join(predicted)
            wrong_moves = " ".join(wrong_moves)
            fh.write(f"{fen}\n{predicted}\n{wrong_moves} | Correct Nodes {correct_nodes}\n")

    print("done")




    close_engine(Engine_1)
    close_engine(Engine_2)