#include "seeder.h"
using ai::getSeeder;
using ai::Seeder;

#include "utils.h"
using ai::idToFilename;

#include "network.h"
using ai::Network;

#include "network-file-io.h"
using ai::NetworkFileReader;
using ai::NetworkFileWriter;

#include "consts.h"
using ai::DEBUG;
using NetworkWeightType = ai::Settings::NetworkWeightType;

#include <algorithm>
#include <vector>
using std::vector;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::ios;

#include <random>
using std::mt19937;
using std::normal_distribution;
using std::uniform_real_distribution;

#include <memory>
using std::shared_ptr;

#include <math.h>

#include <string>
using std::string;

#include <cmath>
using std::abs;

#include <stdexcept>

void Network::save(size_t networkId, Network &network) {
  auto filename = idToFilename(networkId);
  NetworkFileWriter writer;

  writer.save(filename, network);
}

bool Network::load(size_t networkId, Network &network) {
  auto filename = idToFilename(networkId);
  NetworkFileReader reader;

  return reader.load(filename, network);
}

bool Network::load(string &filename, Network &network) {
  NetworkFileReader reader;
  return reader.load(filename, network);
}

Network::Network(unsigned int networkId) : _ID(networkId) {
  load(_ID, *this);
  setupwhichLayerofNetworkToUse();
}

Network::Network(unsigned int networkId,
                 const vector<unsigned int> &layerDimensions,
                 shared_ptr<Seeder> &seeder)
    : _ID(networkId), _performance(0),
      randomNumGenerator(mt19937(seeder->get())) {
  setupLayers(layerDimensions);
  setupRandomWeights(layerDimensions);
  setupKingWeight();
  setupSigmas();
  setupwhichLayerofNetworkToUse();

  save(_ID, *this);
}

void Network::setupLayers(const vector<unsigned int> &layerDimensions) {
  for (auto &layerDimension : layerDimensions) {
    vector<NetworkWeightType> layer(layerDimension, 0);

    _layers.push_back(layer);
  }
}

void Network::setupRandomWeights(const vector<unsigned int> &layerDimensions) {
  auto firstLayerSize = layerDimensions.at(0);

  uniform_real_distribution<NetworkWeightType> distribution(-0.2, 0.2);
  auto firstLayerWeights =
      getRandomNumbersOfLength(firstLayerSize, distribution);
  _weights.push_back(firstLayerWeights);

  for (auto i = 0; static_cast<unsigned int>(i) < layerDimensions.size() - 1; ++i) {
    auto currentLayerDim = layerDimensions.at(i);
    auto nextLayerDim = layerDimensions.at(i + 1);

    auto totalLayerWeights = currentLayerDim * nextLayerDim;
    auto weights = getRandomNumbersOfLength(totalLayerWeights, distribution);

    _weights.push_back(weights);
  }
}

void Network::setupKingWeight() {
  uniform_real_distribution<NetworkWeightType> distribution(1, 3);
  _kingWeight = distribution(randomNumGenerator);
  _pieceCountWeight = distribution(randomNumGenerator);
}

void Network::setupSigmas() {
  _sigmas = _weights;
  for (auto & _sigma : _sigmas) {
    for (unsigned int ii = 0; ii < _sigma.size(); ++ii) {
      _sigma[ii] = .05;
    }
  }
}

void Network::setupwhichLayerofNetworkToUse() {
  vector<int> layerSizes;
  int numLayers = getNumLayers();
  if (numLayers > 1) {
    getLayerSizes(layerSizes);
  }

  _whichLayerofNetworkToUse.resize(25);
  if (numLayers == 1) {
    for (unsigned int i = 0; i < 25; ++i) {
      _whichLayerofNetworkToUse[i].push_back(0);
      _whichLayerofNetworkToUse[i].push_back(_layers.size());
    }
  } else if (numLayers == 2) {
    for (unsigned int i = 9; i < 25; ++i) {
      _whichLayerofNetworkToUse[i].push_back(0);
      _whichLayerofNetworkToUse[i].push_back(layerSizes.at(0));
    }
    for (unsigned int i = 0; i < 9; ++i) {
      _whichLayerofNetworkToUse[i].push_back(layerSizes.at(0));
      _whichLayerofNetworkToUse[i].push_back(layerSizes.at(1) +
                                             layerSizes.at(0));
    }
  } else if (numLayers < 25) {
    int vectorLocationCounter = 24;
    int layerStart = 0;
    int layerEnd = layerSizes[0];
    for (int i = 0; i < numLayers; ++i) {
      for (int ii = 0; ii < 25 / numLayers; ++ii) {
        _whichLayerofNetworkToUse[vectorLocationCounter].push_back(layerStart);
        _whichLayerofNetworkToUse[vectorLocationCounter].push_back(layerEnd);
        --vectorLocationCounter;
      }
      layerStart += layerSizes[i];
      layerEnd = layerStart + layerSizes[i + 1];
    }
    while (vectorLocationCounter != -1) {
      _whichLayerofNetworkToUse[vectorLocationCounter] =
          _whichLayerofNetworkToUse[vectorLocationCounter + 1];
      --vectorLocationCounter;
    }
  } else {
    cout << "I don't handle more than 25 networks" << endl;
  }
}

int Network::getNumLayers() {
  int numLayers = 0;
  for (auto i : _layers) {
    if (i.size() == 1) {
      ++numLayers;
}
  }
  // cout << "numLayers is: " << numLayers << endl;
  return numLayers;
}

void Network::getLayerSizes(vector<int> &layerSizes) {
  int counter = 0;
  int check = 0;
  for (auto i : _layers) {
    ++counter;
    if (i.size() == 1) {
      if (counter < 3) {
        cout << "The counter gave an invalid value" << endl;
      }
      layerSizes.push_back(counter);
      check += counter;
      counter = 0;
    }
  }
  if (check != static_cast<int>(_layers.size())) {
    cout << "Layer sizes don't matchup in getLayerSizes!" << endl;
}
}

template <typename RandomNumberType>
vector<RandomNumberType> Network::getRandomNumbersOfLength(
    const unsigned int length,
    uniform_real_distribution<RandomNumberType> &distribution) {
  vector<RandomNumberType> rngNumbers;
  for (auto i = 0; static_cast<unsigned int>(i) < length; ++i) {
    auto randomNumber = distribution(randomNumGenerator);

    rngNumbers.push_back(randomNumber);
  }
  return rngNumbers;
}

Network::~Network() {
  if (_gameCompleted) {
    // save(_ID, *this);
  }
}

// evaluateBoard returns an answer for red. Flip the sign for black.
NetworkWeightType
Network::evaluateBoard(const vector<char> &inputBoard, bool testing,
                       int red_factor) { // testing defaults false
  // Determine number of pieces in the input board
  int numPieces = 0;
  int pieceCount = 0;
  for (const auto &ii : inputBoard) {
      if (ii == ' ' || ii == 0) {
          continue;
      }

      ++numPieces;
      if (ii == 'r') {
          pieceCount += 1;
      } else if (ii == 'b') {
          pieceCount -= 1;
      } else if (ii == 'R') {
          pieceCount += _kingWeight;
      } else if (ii == 'B') {
          pieceCount -= _kingWeight;
      } else {
          throw "ERROR in Network::evaluateBoard!!!";
      }
  }
  unsigned int layerBeginningIndex = _whichLayerofNetworkToUse[numPieces][0];
  unsigned int layerEndingIndex = _whichLayerofNetworkToUse[numPieces][1];

  /*void inputBoardIntoFirstLayer(inputBoard)*/ {
      int index = 0;
      for (const auto &i : inputBoard) {
          if (i == ' ' || i == 0) {
              _layers[layerBeginningIndex][index] = 0;
          } else if (i == 'r') {
              _layers[layerBeginningIndex][index] = 1;
          } else if (i == 'b') {
              _layers[layerBeginningIndex][index] = -1;
          } else if (i == 'R') {
              _layers[layerBeginningIndex][index] = 1 * _kingWeight;
          } else if (i == 'B') {
              _layers[layerBeginningIndex][index] = -1 * _kingWeight;
          } else {
              cout << "Unrecognized character in board: " << i << endl;
}
          ++index;
      }
  }
  /*void feedForward()*/ {
      for (unsigned int x = layerBeginningIndex + 1; x < layerEndingIndex; ++x) {
          for (unsigned int y = 0; y < _layers[x].size(); ++y) {
              /*calculateNode(x, y)*/ {
                  NetworkWeightType total1 = 0, total2 = 0, total3 = 0, total4 = 0;
                  unsigned int previousLayerSize = _layers[x - 1].size();

                  // Loop unrolling for calculating a node
                  for (unsigned int i = 0; i < previousLayerSize; i += 4) {
                      total1 +=
                          _weights[x][y * previousLayerSize + i] * _layers[x - 1][i];
                      total2 += _weights[x][y * previousLayerSize + i + 1] *
                          _layers[x - 1][i + 1];
                      total3 += _weights[x][y * previousLayerSize + i + 2] *
                          _layers[x - 1][i + 2];
                      total4 += _weights[x][y * previousLayerSize + i + 3] *
                          _layers[x - 1][i + 3];
                  }

                  auto total = total1 + total2 + total3 + total4;

                  _layers[x][y] = (!testing) ? (total / (1 + abs(total))) : total;
              }
          }
      }
  }
  /*boardEvaluationOutput()*/
  return (!testing) ? activationFunction(_layers[layerEndingIndex - 1][0] + _pieceCountWeight * pieceCount) * red_factor
                    : (_layers[layerEndingIndex - 1][0] + _pieceCountWeight * pieceCount) * red_factor;
}

inline NetworkWeightType Network::activationFunction(NetworkWeightType x) {
    // Fast Sigmoid
    return x / (1 + abs(x));
}

void Network::calculateNode(unsigned int x, unsigned int y) {
    NetworkWeightType totalNodeValue = 0;
    unsigned int previousLayerSize = _layers[x - 1].size();

    for (unsigned int i = 0; i < previousLayerSize; i += 2) {
        totalNodeValue +=
            _weights[x][y * previousLayerSize + i] * _layers[x - 1][i];
    }

    _layers[x][y] = totalNodeValue;

    if (DEBUG != 0) {
        cout << _layers[x][y] << endl;
}
}

void Network::adjustPerformance(int adjustment) { _performance = adjustment; }

int Network::getPerformance() const { return _performance; }

void Network::resetPerformance() { _performance = 0; }

void Network::evolveUsingNetwork(Network &rhs) {
    _kingWeight = rhs._kingWeight;
    _weights = rhs._weights;
    _sigmas = rhs._sigmas;
    _pieceCountWeight = rhs._pieceCountWeight;
    this->evolve();
    this->resetPerformance();
    // rhs.resetPerformance();
    save(_ID, *this);
}

void Network::evolve() {
    evolveKingWeight();
    evolveSigmas();
    evolveWeights();
}

NetworkWeightType clamp(NetworkWeightType val, NetworkWeightType lower, NetworkWeightType upper) {
    return std::max(lower, std::min(val, upper));
}

void Network::evolveKingWeight() {
    uniform_real_distribution<NetworkWeightType> distribution(-0.1, 0.1);
    _kingWeight += distribution(randomNumGenerator);

    _kingWeight = clamp(_kingWeight, 1, 3);

    _pieceCountWeight += distribution(randomNumGenerator);
    _pieceCountWeight = abs(_pieceCountWeight);
}

NetworkWeightType Network::getTau() {
    NetworkWeightType numberofWeights = 0;

    for (auto & _weight : _weights) {
        numberofWeights += _weight.size();
    }

    NetworkWeightType tau = 1 / (sqrt(2 * sqrt(numberofWeights)));
    return tau;
}

void Network::evolveSigmas() {
    auto tau = getTau();
    for (size_t i = 0; i < _sigmas.size(); ++i) {
        for (size_t ii = 0; ii < _sigmas[i].size(); ++ii) {
            evolveSigmaAt(i, ii, tau);
        }
    }
}

void inline Network::evolveSigmaAt(size_t i, size_t ii, NetworkWeightType tau) {
    _sigmas[i][ii] = _sigmas[i][ii] * exp(tau * getGaussianNumber(randomNumGenerator));
}

void Network::evolveWeights() {
    for (size_t i = 0; i < _weights.size(); ++i) {
        for (size_t ii = 0; ii < _weights[i].size(); ++ii) {
            _weights[i][ii] = abs(evolveWeightAt(i, ii));
        }
    }
}

NetworkWeightType inline Network::evolveWeightAt(size_t i, size_t ii) {
    return _weights[i][ii] +
        _sigmas[i][ii] * getGaussianNumber(randomNumGenerator);
}

unsigned int Network::getID() const { return _ID; }

void Network::outputCreationDebug() const {
    cout << "Weight for the king: " << _kingWeight << endl;
    cout << "Weight for num pieces: " << _pieceCountWeight << endl;
    cout << "Number of layers: " << _layers.size() << endl;

    for (unsigned int index = 0; index < _layers.size(); ++index) {
        cout << "Size of layer " << index << " = " << _layers[index].size() << endl;
    }

    cout << "size of weights vector: " << _weights.size() << endl;
    for (unsigned int index = 0; index < _weights.size(); ++index) {
        cout << "Size of weight layer " << index << " = " << _weights[index].size()
            << endl;
    }

    cout << "_weights data: " << endl;
    for (auto &v : _weights) {
        for (auto x : v) {
            cout << x << " ";
        }
        cout << endl;
    }

    cout << "_sigmas data: " << endl;
    for (auto &v : _sigmas) {
        for (auto x : v) {
            cout << x << " ";
        }
        cout << endl;
    }
}

void Network::changeKingWeight(NetworkWeightType newWeight) {
    _kingWeight = newWeight;
}

bool ai::operator<(const Network &lhs, const Network &rhs) {
    return lhs.getPerformance() < rhs.getPerformance();
}

bool ai::operator>(const Network &lhs, const Network &rhs) {
    return rhs < lhs;
}

bool ai::operator<=(const Network &lhs, const Network &rhs) {
    return !(lhs > rhs);
}

bool ai::operator>=(const Network &lhs, const Network &rhs) {
    return !(lhs < rhs);
}

bool ai::operator==(const Network &lhs, const Network &rhs) {
    return (lhs._ID == rhs._ID) && (lhs._kingWeight == rhs._kingWeight) &&
        (lhs._performance == rhs._performance) &&
        (lhs._layers == rhs._layers) && (lhs._weights == rhs._weights) &&
        (lhs._pieceCountWeight == rhs._pieceCountWeight) &&
        (lhs._whichLayerofNetworkToUse == rhs._whichLayerofNetworkToUse);
}

void validateNetworkDimensions (const vector<unsigned int>& dimensionsToCheck) {
    int numBeginnings = 0;
    int numEndings = 0;
    for (auto i : dimensionsToCheck) {
        if (i == 32){
            ++numBeginnings;}
        if (i == 1) {
            ++numEndings;}
    }
    if (numBeginnings < numEndings){
        throw std::invalid_argument("Not enough input layers for output layers");}
    if (dimensionsToCheck[0] != 32){
        throw std::invalid_argument("No initial input layer");}
}

void ai::setupNetworks(
        const vector<unsigned int> &dimensions,
        int numberOfNetworks) {
    try {
        validateNetworkDimensions(dimensions);
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
        throw e;
    }

    auto seeder = getSeeder();
    for (auto index = 0; index < numberOfNetworks; ++index) {
        Network(index, dimensions, seeder);
    }
}

NetworkWeightType
ai::getGaussianNumber(std::mt19937 &randomNumGenerator) {
    normal_distribution<NetworkWeightType> distribution(0, 1);

    return distribution(randomNumGenerator);
}

bool ai::nothingSimilar(const Network &lhs, const Network &rhs) {
    bool val = true;
    if (lhs._weights == rhs._weights) {
        cout << "Weights were the same" << endl;
        val = false;
    } else if (lhs._sigmas == rhs._sigmas) {
        cout << "Sigmas were the same" << endl;
        val = false;
    } else if (lhs._ID == rhs._ID) {
        cout << "ID's were the same" << endl;
        val = false;
    } else if (lhs._pieceCountWeight == rhs._pieceCountWeight) {
        cout << "Piece Count Weights were the same" << endl;
        val = false;
    }
    return val;
}

void ai::weightChangeOut(Network parent, Network child) {
    std::ofstream output;
    output.open("weight_change_out.txt", ios::out | ios::trunc);
    for (size_t i = 0; i < parent._weights.size(); i++) {
        for (size_t j = 0; j < parent._weights[i].size(); j++) {
            output << (parent._weights[i][j] - child._weights[i][j]) << endl;
        }
    }
    output.close();
}

bool ai::validateNetworks() {
    vector<Network> allNets;
    for (unsigned int i = 0; i < ai::NETWORKPOPSIZE; ++i) {
        Network net(i);
        allNets.push_back(std::move (net));
    }
    allNets[0].outputCreationDebug();
    vector<char> emptyBoard(32);
    vector<char> sampleBigBoard{
        'r',   'r',   'r',   'r',
    'r',   'r',   'r',   'r',
        'r',   ' ',   'r',   'r',
    ' ',   ' ',   'r',   ' ',
        ' ',   ' ',   'b',   ' ',
    'b',   'b',   ' ',   'b',
        'b',   'b',   'b',   'b',
    'b',   'b',   'b',   'b'
    };
    bool bAreBasicAttributesSame = true;
    bool bAreSmallEvaluationsSame = true;
    ai::Settings::NetworkWeightType emptyEval = allNets[0].evaluateBoard(emptyBoard);
    bool bAreBigEvaluationsSame = true;
    ai::Settings::NetworkWeightType bigEval = allNets[0].evaluateBoard(sampleBigBoard);

    // Check for similarities between networks such that things can be printed once
    for (auto i : allNets) {
        if (bAreSmallEvaluationsSame && i.evaluateBoard(emptyBoard) != emptyEval) {
            bAreSmallEvaluationsSame = false;
}
        if (bAreBigEvaluationsSame && i.evaluateBoard(sampleBigBoard) != bigEval) {
            bAreBigEvaluationsSame = false;
}
        if (!bAreBasicAttributesSame) {
            break;
}

        for (unsigned int index = 0; index < allNets[0]._layers.size(); ++index) {
            if (allNets[0]._layers[index].size() != i._layers[index].size()) {
                bAreBasicAttributesSame = false;
                break;
            }
        }
        if (allNets[0]._weights.size() != i._weights.size()) {
            bAreBasicAttributesSame = false;
        }
        for (unsigned int index = 0; index < i._weights.size(); ++index) {
            if (allNets[0]._weights[index].size() != i._weights[index].size()) {
                bAreBasicAttributesSame = false;
                break;
            }
        }
    }

    if (bAreBasicAttributesSame) {
        cout << "For all networks: " << endl;
        cout << "size of layers and weights vector: " << allNets[0]._weights.size() << endl;
        for (unsigned int index = 0; index < allNets[0]._layers.size(); ++index) {
            cout << "Size of layer " << index << " = " << allNets[0]._layers[index].size()
                << "\tSize of weight layer " << index << " = " << allNets[0]._weights[index].size()
                << endl;
        }
    } else {
        cout << "There's different network sizes in this folder" << endl;
    }

    if (bAreSmallEvaluationsSame) {
        cout << "All networks have empty board output as: " << emptyEval << endl;
    } else {
        cout << "They evaluate empty boards differently" << endl;
    }

    if (bAreBigEvaluationsSame) {
        cout << "All networks have full board output as: " <<  bigEval << endl;
    } else {
        cout << "They evaluate full boards differently" << endl;
    }
    return true;
}
