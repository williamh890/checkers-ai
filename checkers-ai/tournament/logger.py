import sys


def parse(log_file):
    gen = 0

    for line in open(log_file):
        if line[0] != ']':
            continue

        wins = line[1:].split(":")[1].strip()
        total_wins = sum([
            int(network_wins) for network_wins in wins.split(',')
        ])

        print("(gen {}) {} ({})".format(gen, wins, total_wins))
        gen += 1


if __name__ == "__main__":
    try:
        log_file = sys.argv[1]
        print(log_file)
    except:
        print("log-parse.py <log-file-path>")
        print("")
    else:
        parse(log_file)
