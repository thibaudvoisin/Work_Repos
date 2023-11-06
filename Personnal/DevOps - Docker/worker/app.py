# pylint: disable=E0401,C0116,C0114,W0602,C0103,W0702

import os
from flask import Flask
import redis

redis = redis.Redis(
         host= os.environ.get("REDIS_HOST", 'redis'),
         port= '6379')

app = Flask(__name__)

@app.route("/worker")
def root():
    return "Welcome to the worker !"

@app.route("/worker/status")
def status():
    return "ok"

@app.route("/worker/set/<key>/<value>")
def api_set(key, value):
    redis.set(key, value)
    return "set"

@app.route("/worker/get/<key>")
def api_get(key):
    value = redis.get(key)
    return value
