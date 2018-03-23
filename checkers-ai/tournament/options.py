# options file for tournament
# Author: Hal DiMarchi
from configparser import SafeConfigParser
import os


class Options:
    def __init__(self):
        self.config_file = os.path.join(
            os.path.dirname(__file__), "tournament.cfg")
        print(self.config_file)
        self.config = SafeConfigParser()
        self.config.read(self.config_file)
        self.get_options()

    def get_options(self):
        self.run = int(self.config.get('general', 'run'))
        self.user = (self.config.get('general', 'user'))

        self.git_user = self.config.get('git', 'username')
        self.git_password = self.config.get('git', 'password')
        self.repo_url = self.config.get('git', 'url')

        self.network_path = self.config.get('paths', 'network_path')
        self.networks_config = os.path.abspath(
            os.path.join(self.network_path, self.user, "generations.cfg"))

        self.checkers_path = self.config.get('paths', 'checkers_path')
        checkers = self.config.get('functions', 'checkers_game')
        self.checkers_game = os.path.join(self.checkers_path,
                                          checkers)
        network_manager = self.config.get('functions', 'network_manager')
        self.network_count = int(self.config.get('functions', 'network_count'))
        self.network_manager = os.path.join(self.checkers_path,
                                            network_manager)
        self.max_processes = int(self.config.get('processes', 'max_processes'))

    def check_run(self):
        self.config.read(self.config_file)
        self.run = int(self.config.get('general', 'run'))
