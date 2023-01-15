import argparse
import pandas


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input")
    parser.add_argument("-o", "--output")

    args = parser.parse_args()

    input_data = pandas.read_csv(args.input)
    with open(args.output, "wt") as f:
        n = 0
        for index, row in input_data.iterrows():
            n = max(n, int(row['src']), int(row['dst']))

        f.write(f"{n + 1} {input_data.shape[0]}\n")
        for index, row in input_data.iterrows():
            f.write(f"{row['src']} {row['dst']}\n")


if __name__ == '__main__':
    main()
