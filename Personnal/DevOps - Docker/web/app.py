# pylint: disable=E0401,C0116,C0114,W0602,C0103,W0702

import os
from flask import Flask
import redis

app = Flask(__name__)

@app.route("/")
def root():
    return "Welcom to awesome app"

@app.route("/status")
def status():
    return "ok"

@app.route("/check_redis")
def check_redis():
    try:
        global redis
        redis_co = redis.Redis(
            host= os.environ.get("REDIS_HOST", 'redis'),
            port= os.environ.get("REDIS_PORT", '6379'))
        redis_co.ping()
        return "KO", 500
    except:
        return "OK"
