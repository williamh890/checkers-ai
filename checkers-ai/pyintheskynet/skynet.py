import requests
import sys
import json
import argparse

endpoints = [
    'play_game',
    'game_info'
]

local_url = "http://localhost:8080"
skynet_url = "http://skynet.cs.uaf.edu"

first_move = "rrrrrrrr_rrrr_______bbbbbbbbbbbb"

parser = argparse.ArgumentParser(
    description='python interface for making calls to skynet api'
)

parser.add_argument(
    'endpoint',
    help='the skynet endpoint to be called [{}]'.format('|'.join(endpoints))
)

parser.add_argument(
    '--game', help='name of the skynet game',
    dest='game'
)

parser.add_argument(
    '--board', help='board string in skynet format',
    dest='board'
)

parser.add_argument(
    '--server',
    dest='server',
    default=local_url,
    help='url of the skynet server'
)


def main():
    args = parser.parse_args()
    net = Skynet(args.server)

    if not args.endpoint in endpoints:
        print("invalid endpoint {}".format(args.endpoint))
        exit()

    if args.endpoint == 'game_info':
        info = net.game_info(args.game)
        print(info)

    if args.endpoint == 'play_game':
        output = net.play_game(args.game, args.board)
        print(output)


class Skynet:
    def __init__(self, url):
        self.url = url

    def game_info(self, game_name):
        endpoint_url = self.get_endpoint_url('info_game')

        resp = requests.post(
            endpoint_url,
            json={"name": game_name}
        )

        return json.loads(resp.text)

    def play_game(self, game_name, board_str):
        endpoint_url = self.get_endpoint_url('play_game')

        resp = requests.post(
            endpoint_url,
            json={
                "name": game_name,
                "board": board_str
            }
        )

        return json.loads(resp.text)

    def get_endpoint_url(self, endpoint):
        return "{url}/?{endpoint}=true".format(
            url=self.url,
            endpoint=endpoint
        )


if __name__ == "__main__":
    main()
