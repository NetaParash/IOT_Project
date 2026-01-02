import json
import os
from enum import Enum

# Load shared configuration from JSON
env = os.getenv('ENVIRONMENT', 'dev')
config_path = os.path.join(os.path.dirname(__file__), f'../{env}.json')
with open(config_path) as config_file:
    shared_config_data = json.load(config_file)


class CacheKey:
    def __init__(self, key, expiration):
        self.key = key
        self.expiration = expiration


DAY_IN_SECONDS = 86400


class CacheKeys(Enum):