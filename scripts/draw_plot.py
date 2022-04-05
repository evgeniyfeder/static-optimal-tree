import plotly.express as px
import pandas
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", help="input comparison csv")
    parser.add_argument("-o", "--output", help="output file for plot")
    args = parser.parse_args()

    input_data = pandas.read_csv(args.input)
    fig = px.scatter(input_data, x="step", y=["optimal","period","splaynet"])
    fig.show()

if __name__ == '__main__':
    main()