import requests
import shutil


def download_file(url):
    local_filename = url.split('/')[-1]
    r = requests.get(url, stream=True)
    with open(local_filename, 'wb') as f:
        shutil.copyfileobj(r.raw, f)

    return local_filename


if __name__ == "__main__":
    f = 'https://dl.bintray.com/boostorg/' + \
        'release/1.66.0/source/boost_1_66_0.tar.gz'
    print('downloading from {}'.format(f))
    download_file(f)
