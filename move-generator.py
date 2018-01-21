import json


class SpaceMoves:
    def __init__(self, jumps, moves):
        self.jumps = jumps
        self.moves = moves

    def get_all_moves_at(self, space):
        return self.jumps[space] + self.moves[space]

    def get_jumps_at(self, space):
        return self.jumps[space]

    def get_moves_at(self, space):
        return self.moves[space]


class CheckersMoveGenerator:
    def __init__(self, black=None, red=None):
        self.black = black
        self.red = red

    def get_red_moves(self, space):
        return self.red.get_all_moves_at(space)

    def get_black_moves(self, space):
        return self.black.get_all_moves_at(space)

    def get_king_moves(self, space):
        return self.black.get_jumps_at(space) + \
            self.red.get_jumps_at(space) + \
            self.black.get_moves_at(space) + \
            self.red.get_moves_at(space)


def get_move_generator(movesfile):
    with open(movesfile) as f:
        moves = json.loads(f.read())

    r_jumps, r_moves = get_moves_and_jumps('red', moves)
    b_jumps, b_moves = get_moves_and_jumps('black', moves)

    red = SpaceMoves(r_jumps, r_moves)
    black = SpaceMoves(b_jumps, b_moves)

    return CheckersMoveGenerator(black=black, red=red)


def get_moves_and_jumps(color, d):
    jumps, moves = d[color]['jumps'], d[color]['moves']

    jumps_with_nums = make_keys_to_ints(jumps)
    moves_with_nums = make_keys_to_ints(moves)

    return (jumps_with_nums, moves_with_nums)


def make_keys_to_ints(d):
    d_with_nums = {}
    for k, v in d.items():
        d_with_nums[int(k)] = v

    return d_with_nums


if __name__ == "__main__":
    get_move_generator('move-table.json')
