N=25;
M=1000;

cd build-debug || exit;
make;
cd ..
for (( i = 6; i < 8; i++ )); do
  build-debug/main $i $M
  pipenv run python drawgraph.py
done
