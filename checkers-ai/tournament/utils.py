from os.path import join, exists, abspath as ap


def ensure_generation_cfg(cfg_file):
    if exists(ap(cfg_file)):
        return

    with open(cfg_file, 'w') as config_file:
        config_file.write("[generations] \ncount = 0")
