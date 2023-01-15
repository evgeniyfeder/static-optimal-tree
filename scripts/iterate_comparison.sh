NUMBER_SAMPLES=1;
DIR_NAME=/home/evgeniyfeder/Downloads/itmo/static-optimal-tree/scripts/real_traces;
TRACES=("pfabric" "fb" "hpc" "uni" "ptor" "mic");

mkdir ${DIR_NAME}_csv;

for p_id in ${TRACES[@]}; do
  for (( sample_id = 0; sample_id < NUMBER_SAMPLES; sample_id++ )); do
      /home/evgeniyfeder/Downloads/itmo/static-optimal-tree/build/opt_pair_tree-comparison ${DIR_NAME}/sample_${p_id}_${sample_id}.in ${DIR_NAME}_csv/sample_${p_id}_${sample_id}.csv
  done
done