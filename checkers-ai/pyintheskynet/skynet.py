import requests
import sys


def get_game_info(url):
    endpoint = '?info_game=true'

    endpoint_url = "{url}/{endpoint}".format(url=url, endpoint=endpoint)
    print(endpoint_url)
    resp = requests.post(
        endpoint_url,
        json={"name": "test"}
    )

    print(resp.text)


if __name__ == "__main__":
    url = sys.argv[1]

    game_data = get_game_info(url)
