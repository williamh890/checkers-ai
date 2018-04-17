import sys


def main(fname):
    total, num = 0, 0
    for log_line in open(fname):
        val = int(log_line.split(":")[-1].strip())
        print(val)
        total += val
        num += 1

    print(total / num)


if __name__ == "__main__":
    main(sys.argv[1])
