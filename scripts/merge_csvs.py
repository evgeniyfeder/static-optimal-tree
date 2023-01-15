import csv
import pathlib

TRACES = {"fb", "hpc", "mic", "pfabric", "ptor", "uni"}
NUM_SAMPLES = 1
DIR_NAME="/home/evgeniyfeder/Downloads/itmo/static-optimal-tree/scripts/real_traces"

CSV_BASE_PATH = pathlib.Path(f"{DIR_NAME}_csv/sample.csv")
CSV_MERGED_PATH = pathlib.Path(f"{DIR_NAME}_merged/sample.csv")
CSV_MERGED_PATH.parents[0].mkdir(parents=True, exist_ok=True)

for name in TRACES:
    with open(CSV_MERGED_PATH.with_stem(f"{CSV_MERGED_PATH.stem}_{name}"), "w") as out_f:
        info = {}

        for i in range(0, NUM_SAMPLES):
            with open(CSV_BASE_PATH.with_stem(f"{CSV_BASE_PATH.stem}_{name}_{i}")) as in_f:
                csv_reader = csv.DictReader(in_f)

                # Make average
                for line in csv_reader:
                    info[line["step"]] = {
                                          "splaynet": info.get(line["step"], {}).get("splaynet", 0) + float(line["splaynet"]),
                                          "central_splaynet": info.get(line["step"], {}).get("central_splaynet", 0) + float(line["central_splaynet"]),
                                          "full_binary": info.get(line["step"], {}).get("full_binary", 0) + float(line["full_binary"]),
                                          "static_optimal": info.get(line["step"], {}).get("static_optimal", 0) + float(line["static_optimal"])
                                          }
        csv_writer = csv.DictWriter(out_f, ["step", "splaynet", "central_splaynet", "full_binary", "static_optimal"])
        csv_writer.writeheader()

        for step, row in info.items():
            res = {
                "step": step,
                "splaynet": row["splaynet"] / NUM_SAMPLES,
                "central_splaynet": row["central_splaynet"] / NUM_SAMPLES,
                "full_binary": row["full_binary"] / NUM_SAMPLES,
                "static_optimal": row["static_optimal"] / NUM_SAMPLES
            }
            csv_writer.writerow(res)