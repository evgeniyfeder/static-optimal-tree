import plotly.express as px
import pandas
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", help="input comparison csv")
    parser.add_argument("-o", "--output", help="output file for plot")
    args = parser.parse_args()

    input_data = pandas.read_csv(args.input)
    input_data["SplayNet"] = input_data["splaynet"]
    input_data["3-SplayNet"] = input_data["central_splaynet"]
    input_data["Full Binary Net"] = input_data["full_binary"]
    input_data["Static Optimal Net"] = input_data["static_optimal"]
    fig = px.line(input_data, x="step", y=["SplayNet","3-SplayNet", "Full Binary Net", "Static Optimal Net"], log_y=True)
    fig.update_layout(xaxis_title="Number of operations",
                      yaxis_title="Average routing cost",
                      legend_title="Network algorithm")
    fig.write_image(args.output)

if __name__ == '__main__':
    main()