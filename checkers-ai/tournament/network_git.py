from git import Repo
from configparser import ConfigParser
from os.path import dirname as dn, abspath as ap


class NetworkGit:
    def __init__(self, networks_config, user, git_user, git_password, repo_url):
        self.get_push_command(git_user, git_password, repo_url)
        self.user = user
        self.networks_config = networks_config
        self.generations = ConfigParser()
        self.generations.read(ap(self.networks_config))
        self.current_generation = int(self.generations.get('generations',
                                                           'count'))
        self.network_repo = Repo(dn(dn(self.networks_config)))

    def get_push_command(self, user, password, url):
        self.push_command = "https://{}:{}@github.com/{}".format(user,
                                                                 password,
                                                                 url)

    def update_remote(self):
        self.network_repo.git.add(all=True)

        commit_msg = "generation {} of user {}".format(
            self.current_generation,
            self.user
        )
        self.network_repo.git.commit("-m {}".format(commit_msg))

        with open("user.me", "r") as f:
            branch = f.read().strip()

        self.network_repo.git.push(self.push_command, branch)

    def update_config(self):
        self.current_generation += 1

        with open(self.networks_config, 'w') as network_config:
            self.generations.set("generations",
                                 "count",
                                 str(self.current_generation))
            self.generations.write(network_config)
