import argparse
import json

from pathlib import Path
from zipfile import ZipFile
from enum import Enum


class ResourceType(Enum):
    NONE = 0
    TEXTURE2D = 1
    SHADER = 2


class ResourceConfig:
    def __init__(self, res_id: str, name: str, res_type: str, data: dict):
        assert res_id, "Resource ID shouldn't be empty"
        assert name, "Resource name shouldn't be empty"
        assert type, "Resource type shouldn't be empty"
        assert data, "Resource data shouldn't be empty"
        self.type = ResourceType[res_type.upper()]
        if self.type == ResourceType.TEXTURE2D:
            assert 'file' in data, "Resource data of TEXTURE2D should contain 'file'"
        elif self.type == ResourceType.SHADER:
            assert 'vertex' in data, "Resource data of SHADER should contain 'vertex'"
            assert 'fragment' in data, "Resource data of SHADER should contain 'fragment'"
        self.id = res_id
        self.name = name
        self.data = data


class Config:
    def __init__(self, path: Path):
        self.config_path = path
        f = open(path, 'r')
        data = json.loads(f.read())
        f.close()

        self.resources = list()
        ids = list()
        names = list()
        for res in data:
            resource = ResourceConfig(res['id'], res['name'], res['type'], res['data'])
            assert resource.id not in ids
            assert resource.name not in names
            ids.append(resource.id)
            names.append(resource.name)
            self.resources.append(resource)

    def get_files(self):
        result = list()
        result.append(self.config_path)
        path = self.config_path.parent
        for resource in self.resources:
            for file_path in resource.data.values():
                result.append(path / file_path)
        return result


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
