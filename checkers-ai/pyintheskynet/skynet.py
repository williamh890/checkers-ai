import requests
import sys
import json


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
    local_url = "http://localhost:8080"
    skynet_url = "http://skynet.cs.uaf.edu"

    first_move = "rrrrrrrr_rrrr_______bbbbbbbbbbbb"

    net = Skynet(local_url)

    name = sys.argv[1]

    print(net.play_game(name, first_move))
