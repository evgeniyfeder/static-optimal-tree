TRACES=("pfabric" "fb" "hpc" "ptor" "mic" "uni");
#TRACES=(0 0.25 0.5 0.75 0.9)
DIR_NAME=/home/evgeniyfeder/Downloads/itmo/static-optimal-tree/scripts/real_traces;

for p_id in ${TRACES[@]}; do
  /home/evgeniyfeder/.local/share/virtualenvs/static-optimal-tree-hv4LcdL4/bin/python draw_plot.py -i ${DIR_NAME}_merged/sample_${p_id}.csv -o ${DIR_NAME}_merged/sample_${p_id}.png
done