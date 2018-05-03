#define SETUP_SEACH_VARIABLES() \
    auto isMaximizingPlayer = \
        game.activePlayer->getColor() == maximizingPlayer;    \
    float best = (isMaximizingPlayer) ? -INF : INF;           \
    float bestVal;

#define SEARCH_ACTIONS(actions, depthExpr, cmpFunc, toUpdate) \
    for (auto &action : actions) {                            \
        bestVal = recurse(action, depthExpr, alpha, beta);    \
                                                              \
        bestOverallVal = cmpFunc(bestVal, best);              \
        toUpdate = cmpFunc(toUpdate, best);                   \
                                                              \
        if (beta <= alpha) {                                  \
            ++prunedNodes;                                    \
            break;                                            \
        }                                                     \
    }

#define SEARCH_BASED_ON_MAXIMIZING_PLAYER(actions, depthExpr) \
    if (isMaximizingPlayer) {                                 \
        SEARCH_ACTIONS(actions, depthExpr, max, alpha);       \
    } else {                                                  \
        SEARCH_ACTIONS(actions, depthExpr, min, beta);        \
    }
