import multiprocessing
import time
import subprocess

cmd = "sudo docker run train"


def main():
    pool = multiprocessing.Pool(processes=8)

    start = time.time()
    pool.map_async(
        run,
        [cmd for _ in range(8)]
    ).get()
    end = time.time()

    print("Took : {} secs".format(end - start))


def run(cmd):
    print(cmd)
    ret, program_output = subprocess.getstatusoutput(cmd)
    print(program_output)


if __name__ == "__main__":
    main()
