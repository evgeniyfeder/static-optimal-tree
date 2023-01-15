# Pipeline for generating plot charts

To generate plots you need to do the following steps:
1. Get data and transfer it to the comparator format 
2. Run comparator for each trace
3. Merge samples for each trace
4. Generate charts

## Get data and transfer it to the comparator format 
You have a lot of different sources of data. Here I have 3 scripts for transferring different sources.
``transfer_roberty_input.py``, ``transfert_real_input.py`` and  ''sample_random.py''.
The output format in each script is the following file:
```
N M
from_1 to_1
...
from_M to_M
```
Where node id is in range [0,N-1]. To run python scripts you need to make environment. You have Pipfile for make virtual environment. Example Usages:

```
transfer_real_input.py -i real_traces/Microsoft.csv -o real_traces/sample_mic_0.in
transfer_robert_input.py --input p04+p06.json --output-prefix robert_0406/sample.in
sample_random.py -n 1000 -m 100000 -o real_traces/sample_uni_0.in 
```

## Run comparator for each trace

Here you need to open CMake project and compile ```opt_pair_tree-comparison``` target.
Sometimes you have a lot of trace samples, so for each sample I used format ```<collection_name>/sample_<trace_name>_<sample_id>.in```.
With these assumptions you can open ``iterate_comparison.sh``, correct all variables and run.
Result file for each run will be ``<collection_name>_csv/sample_<trace_name>_<sample_id>.csv``.

## Merge samples for each trace
In that moment for each trace you have a lot of samples (usually it's 10). So you need to merge them. Open ``merge_csvs.py``, Correct global variables and run it. Result file name of each trace will be:
``<collection_name>_merged/sample_<trace_name>.csv``

## Generate charts
Then you need to make charts. See to ``iterate_plots.sh`` to get it. Result is in ``<collection_name>_merged/sample_<trace_name>.png`` file.