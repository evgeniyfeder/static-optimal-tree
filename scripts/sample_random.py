import argparse
from random import randrange


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", default=1000, type=int)
    parser.add_argument("-m", default=100000, type=int)
    parser.add_argument("-o", "--output")

    args = parser.parse_args()

    with open(args.output, "wt") as f:
        f.write(f"{args.n} {args.m}\n")
        for i in range(args.m):
            f.write(f"{randrange(args.n)} {randrange(args.n)}\n")


if __name__ == '__main__':
    main()
