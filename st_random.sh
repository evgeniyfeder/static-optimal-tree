M=1000;
CUR=10;
NUM=10;
cd build-debug || exit;
make;
cd ..
for (( i = $CUR; i < $CUR + $NUM; i++ )); do
  build-debug/opt_pair_tree $i $M
  pipenv run python drawgraph.py
done
