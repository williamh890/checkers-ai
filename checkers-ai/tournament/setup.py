from configparser import SafeConfigParser
import subprocess
import pip
import os

NETWORK_MANAGER = "./manager.out"
TOURNAMENT_CFG = "tournament.cfg"
SETUP_NETWORKS = "{} 0".format(NETWORK_MANAGER)
REPO_URL = "williamh890/checkers-ai-networks.git"


def add_generations_cfg(checkers_path, network_path):
    with open(checkers_path + '/user.me') as f:
        username = f.read().strip()

    generations_path = os.path.join(network_path, username, 'generations.cfg')
    if os.path.isdir(generations_path):
        return

    gens = SafeConfigParser()
    gens['generations'] = {'count': 0}

    with open(generations_path, 'w') as f:
        gens.write(f)


if __name__ == "__main__":
    pip.main(['install', '-r' 'requirements.txt'])

    print("\n")
    print("Tournament Settings: ")
    print('-' * 40)

    print('networks')
    new_networks = input("  setup new networks(y/n): ")

    if new_networks == 'y':
        output = subprocess.getstatusoutput(
            "cd .. && {} 0".format(NETWORK_MANAGER))
        print(output[1])

    config = SafeConfigParser()
    config['general'] = {'run': 1}
    config['functions'] = {'checkers_game': 'checkers.out',
                           'network_manager': 'manager.out',
                           'network_count': 30}
    print("absolute path")
    checkers_path = input("  'checkers-ai' folder: ")
    network_path = input("  'networks' folder: ")

    config['paths'] = {'network_path': network_path,
                       'checkers_path': checkers_path}

    add_generations_cfg(checkers_path, network_path)

    print("git")
    git_username = input("  username: ")
    git_password = input("  password: ")

    config["git"] = {'username': git_username,
                     'password': git_password,
                     'url': REPO_URL}

    print("processes")
    while True:
        processes = input("  number of concurrent games to run: ")
        try:
            config["processes"] = {"max_processes": int(processes)}
        except Exception as e:
            print(e)
        else:
            break

    with open(TOURNAMENT_CFG, 'w') as cfgfile:
        config.write(cfgfile)

    print("Done generating config file")
