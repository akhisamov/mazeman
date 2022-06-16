import argparse

from pathlib import Path
from zipfile import ZipFile


class Config:
    def __init__(self, path: Path):
        f = open(path, 'r')

        self.config_path = path
        self.dict = {}
        last_key = ""
        for line in f.readlines():
            line_wo_n = line.splitlines().pop(0)
            if line_wo_n.startswith('[') and line_wo_n.endswith(']'):
                last_key = line_wo_n[1:-1]
                self.dict[last_key] = list()
            else:
                self.dict[last_key].append(line_wo_n)

        f.close()

    def get_files(self):
        files = list()
        files.append(self.config_path)
        path = self.config_path.parent
        for value in self.dict.values():
            for file_path in value:
                files.append(path / file_path)
        return files


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Bundle builder")
    parser.add_argument('input', type=Path, help="Input Path")
    parser.add_argument('output', type=Path, help="Output Path")
    args = parser.parse_args()
    assert args.input.is_file() and args.input.suffix == '.bundleconf', 'input path should be to the .bundleconf'
    config = Config(args.input)
    print('Starting to pack bundle')
    bundle_path = args.output / (args.input.stem + '.bundle')
    with ZipFile(bundle_path, 'w') as bundle:
        files = config.get_files()
        i = 1
        for file in config.get_files():
            print('[{}/{}] Packing: \'{}\' to \'{}\''.format(i, len(files), file, bundle_path))
            bundle.write(file, file.relative_to(args.input.parent))
            i += 1
        bundle.close()
    print('Finished to pack bundle')
