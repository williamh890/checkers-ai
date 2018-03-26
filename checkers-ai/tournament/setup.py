from configparser import SafeConfigParser
import subprocess
import pip

NETWORK_MANAGER = "./manager.out"
TOURNAMENT_CFG = "tournament.cfg"
SETUP_NETWORKS = "{} 0".format(NETWORK_MANAGER)
REPO_URL = "williamh890/checkers-ai-networks.git"

if __name__ == "__main__":
    pip.main(['install', '-r' 'requirements.txt'])

    new_networks = input("would you like to setup new networks?(y/n)?")
    if new_networks == 'y':
        output = subprocess.getstatusoutput("cd .. && {} 0".format(NETWORK_MANAGER))
        print(output[1])

    config = SafeConfigParser()
    config['general'] = {'run': 1}
    config['functions'] = {'checkers_game': 'checkers.out',
                           'network_manager': 'manager.out',
                           'network_count': 30}

    checkers_path = input("give me the absolute path to your 'checkers-ai' folder")
    network_path = input("give me the absolute path to your 'networks' folder")
    config['paths'] = {'network_path': network_path,
                       'checkers_path': checkers_path}

    git_username = input("give me your git username")
    git_password = input("give me your git password")

    config["git"] = {'username': git_username,
                     'password': git_password,
                     'url': REPO_URL}
    with open(TOURNAMENT_CFG, 'w') as cfgfile:
        config.write(cfgfile)

    print("Done generating config file")
