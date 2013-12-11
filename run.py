#!/usr/bin/env python3
import time
import itertools
import multiprocessing
import threading
import os

RANGES = {
  "traffic.lambda": [0.0001 * i for i in range(20, 61)],
}

def get_param(k, v):
  if isinstance(v, float):
    return (k, "%.6f" % v)
  return (k, str(v))

lists = itertools.product(*[[get_param(k, v) for v in RANGES[k]] for k in RANGES])

semaphore = threading.Semaphore(multiprocessing.cpu_count())

def exe(param):
  global semaphore
  with semaphore:
    filename = ",".join(["{}_{}".format(p[0], p[1]) for p in param])
    param_list = " ".join(["--{}={}".format(p[0], p[1]) for p in param])
    os.system("./umnets {} > data/{}".format(param_list, filename))

os.system("mkdir -p data")
os.system("cp config.cfg data/")
for param in lists:
  threading.Thread(target = exe, args = (param, )).start()

