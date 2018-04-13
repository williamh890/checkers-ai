from configparser import SafeConfigParser
import subprocess
import pip
import os
import getpass

NETWORK_MANAGER = "./manager.out"
TOURNAMENT_CFG = "tournament.cfg"
SETUP_NETWORKS = "{} 0".format(NETWORK_MANAGER)
REPO_URL = "williamh890/checkers-ai-networks.git"


def setup_networks(checkers_path, network_path):
    while True:
        network_user = input('  network folder name: ').strip()

        if os.path.exists(os.path.join(network_path, network_user)):
            print("folder name {} already exists in networks directory".format(
                network_user))
            continue

        try:
            path = os.path.join(checkers_path, 'user.me')

            print(path)
            with open(path, 'w') as f:
                f.write(network_user)
        except Exception as e:
            print(e)
        else:
            break

    output = subprocess.getstatusoutput(
        "cd .. && {} 0".format(NETWORK_MANAGER))
    print("NETWORK MANAGER")
    print(output[1])
    print("DONE")
    print("")

    generations_path = os.path.join(
        network_path,
        network_user,
        'generations.cfg'
    )
    if os.path.isdir(generations_path):
        return

    gens = SafeConfigParser()
    gens['generations'] = {'count': 0}

    with open(generations_path, 'w') as f:
        gens.write(f)


def clone_networks_repo(network_path):
    os.system('git clone https://www.github.com/{repo} {path}'.format(
        repo=REPO_URL, path=network_path
    ))


if __name__ == "__main__":
    pip.main(['install', '-r' 'requirements.txt'])

    print("\n")
    print("Tournament Settings: ")
    print('-' * 40)

    config = SafeConfigParser()
    config['general'] = {'run': 1}
    config['functions'] = {'checkers_game': 'checkers.out',
                           'network_manager': 'manager.out',
                           'network_count': 100}

    checkers_path = '/' + os.path.join(*os.getcwd().split('/')[:-1])
    network_path = os.path.join(checkers_path, 'networks')

    print("absolute path")
    input_checkers_path = input(
        "  'checkers-ai' folder (default is {}): ".format(checkers_path)
    )
    if input_checkers_path != "":
        print("using default")
        checker_path = input_checkers_path

    input_network_path = input(
        "  'networks' folder (default is {}): ".format(network_path))

    if input_network_path != "":
        network_path = input_network_path

    config['paths'] = {'network_path': network_path,
                       'checkers_path': checkers_path}

    print('networks')
    shoule_clone_networks = input("  clone networks repo(y/n): ")
    if shoule_clone_networks == 'y':
        clone_networks_repo(network_path)

    new_networks = input("  setup new networks(y/n): ")

    if new_networks == 'y':
        setup_networks(checkers_path, network_path)

    while True:
        games_per_match = input("  games per match (default 3): ")
        if games_per_match == "":
            games_per_match = 1
        try:
            games_per_match = int(games_per_match)
        except:
            print("  enter a number...")
        else:
            break

    search_depth = input(" move time limit (default 5): ")
    if search_depth == "":
        search_depth = 5.

    config["match"] = {
        "games_per_match": int(games_per_match),
        "search_depth": float(search_depth)
    }

    print("git")
    git_username = input("  username: ")
    git_password = getpass.getpass("  password: ")

    config["git"] = {
        'username': git_username,
        'password': git_password,
        'url': REPO_URL
    }

    print("processes")
    while True:
        processes = input("  number of concurrent games to run: ")
        try:
            config["processes"] = {
                "max_processes": int(processes)
            }
        except Exception as e:
            print(e)
        else:
            break

    with open(TOURNAMENT_CFG, 'w') as cfgfile:
        config.write(cfgfile)

    print("Done generating config file")
