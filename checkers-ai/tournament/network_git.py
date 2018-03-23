from git import Repo
from configparser import ConfigParser
from os.path import join, dirname as dn, abspath as ap


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
        print(self.push_command)

    def update_remote(self):
        self.network_repo.git.add(all=True)
        self.network_repo.git.commit("-m generation {} of user {} ".format(
            self.current_generation, self.user))
        print("pushing, push command is {}".format(self.push_command))
        options = {"repo": self.push_command, "all": True}
        self.network_repo.git.push(porcelain=True,
                                   as_process=True,
                                   universal_newlines=True,
                                   **options)

    def update_config(self):
        self.current_generation += 1
        with open(self.networks_config, 'w') as network_config:
            self.generations.set("generations",
                                 "count",
                                 str(self.current_generation))
            self.generations.write(network_config)
