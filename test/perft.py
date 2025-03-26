from subprocess import Popen, PIPE

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
            
def parse_output(output: str) -> list:
    output = output.split("\n")
    output = [line for line in output if ((": ") in line) and (("info") not in line)]
    # Nodes will always be first, then the rest will be moves
    output = sorted(list(set(output)))
    return output

def close_engine(Engine: Popen) -> None:
    Engine.stdin.write("quit\n".encode())
    Engine.stdin.flush()

if __name__ == "__main__":
    # reference engine
    Engine_1 = Popen([f'./{Engine_1_path}'], stdout=PIPE, stdin=PIPE)
    # your engine
    Engine_2 = Popen([f'./{Engine_2_path}'], stdout=PIPE, stdin=PIPE)
    output = send_command(Engine_1, "go perft 3")
    print(parse_output(output))
    output = send_command(Engine_2, "go perft 3")
    print(parse_output(output))


    close_engine(Engine_1)
    close_engine(Engine_2)