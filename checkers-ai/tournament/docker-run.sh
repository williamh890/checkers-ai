
# Setup networks
cd ..
    git clone https://www.github.com/williamh890/networks
    echo $(uuid) > user.me
    cd networks && git checkout -b $(cat ../user.me) && cd ..
    ./manager.out 0
cd tournament

# Start the tournament
python3 director.py

