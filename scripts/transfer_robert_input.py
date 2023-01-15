import argparse
import json
import pathlib


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument("--input", type=pathlib.Path, help="path to json workload data")
    parser.add_argument("--output-prefix", type=pathlib.Path, help="path to the file for net comparison")

    args = parser.parse_args()
    args.output_prefix.parents[0].mkdir(parents=True, exist_ok=True)

    with open(args.input) as input_f:
        input_json = json.load(input_f)
        print(input_json.keys())

        for p, traces in input_json.items():
            for trace_i, trace in traces["sequences"].items():
                with open(args.output_prefix.with_stem(f"{args.output_prefix.stem}_{p}_{trace_i}"), "w") as out_f:
                    n = 0
                    pairs = []
                    if isinstance(trace, dict):
                        for pair_i, pair in trace.items():
                            n = max(n, pair[0], pair[1])
                            pairs.append(pair)
                    elif isinstance(trace, list):
                        for pair in trace:
                            n = max(n, pair[0], pair[1])
                        pairs = trace

                    out_f.write(f"{n} {len(pairs)}\n")

                    for pair in pairs:
                        out_f.write(f"{pair[0] - 1} {pair[1] - 1}\n")


if __name__ == '__main__':
    main()
