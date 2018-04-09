import json


def parse_game(game_output):
    move_times, game_time = [], None

    for line in game_output.split('\n'):
        try:
            timing_type, time = line.split(':')
        except Exception:
            continue

        if timing_type == "move":
            move_times.append(time)
        elif timing_type == "game":
            game_time = time

    return {
        "moves": len(move_times),
        "total": game_time,
        "unit": "seconds"
    }


def write_to_log(generation, game_results, network_wins):
    log_dict = {
        "session gen": generation,
        "results":  game_results,
        "wins": network_wins
    }

    with open("tournaments.log", "a") as f:
        f.write(json.dumps(log_dict))


if __name__ == "__main__":
    output = parse_game(open("test.log").read())
    print(json.dumps(output, indent=2))
