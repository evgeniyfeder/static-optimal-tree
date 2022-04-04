import argparse
import pathlib

import pandas
import attr


@attr.s(auto_attribs=True)
class SplayNetOutput:
    t: int
    lazy_route_cost: int
    lazy_adj_cost: int
    splay_cost: int


def parse_splaynet_output(out_path: pathlib.Path):
    result = []
    with out_path.open() as f:
        for idx, line in enumerate(f):
            values = [int(n) for n in line.split()]

            result.append(
                SplayNetOutput(t=idx + 1,
                               lazy_route_cost=values[0],
                               lazy_adj_cost=values[1],
                               splay_cost=values[2]))
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--splaynet", type=pathlib.Path, help="Path to file with logs from splaynet executable")
    parser.add_argument("--cur-logs", type=pathlib.Path, help="Path to file with logs from comparison")
    parser.add_argument("--output", help="Path to the output file")

    args = parser.parse_args()

    cur_logs = pandas.read_csv(args.cur_logs)
    splaynet_logs = {elem.t: elem for elem in parse_splaynet_output(args.splaynet)}

    timestamps = []
    for t in cur_logs['step']:
        timestamps.append(t)

    print(timestamps)
    cur_logs["splaynet"] = [splaynet_logs[t].splay_cost / t for t in timestamps]

    cur_logs.to_csv(args.output, index=False)

if __name__ == '__main__':
    main()
