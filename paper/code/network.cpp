// Use for to pick the right network depending
// on the stage of the game.
for (size_t x = begIdx + 1; x < endIdx; ++x) {

    // Look through the layers in the network
    for (size_t y = 0; y < _layers[x].size(); ++y) {
        size_t prevSize = _layers[x - 1].size();
        float t1 = 0, t2 = 0, t3 = 0, t4 = 0;

        // Loop unrolling for calculating a node
        for (size_t i = 0; i < prevSize; i += 4) {
            // Calculate nodes value
            t1 += _weights[x][y * prevSize + i] *
                _layers[x - 1][i];
            t2 += _weights[x][y * prevSize + i + 1] *
                _layers[x - 1][i + 1];
            t3 += _weights[x][y * prevSize + i + 2] *
                _layers[x - 1][i + 2];
            t4 += _weights[x][y * prevSize + i + 3] *
                _layers[x - 1][i + 3];
        }

        // Total up from unrolled loop
        auto total = t1 + t2 + t3 + t4;

        // Store value for that node
        _layers[x][y] = total / (1 + abs(total));
    }
}

float withPieceCount = _layers[endIdx - 1][0] + pieceCount;

return activation(withPieceCount) * red_factor;
