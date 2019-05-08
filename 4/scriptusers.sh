#!/bin/bash

exit $(ps -U "$1" | wc -l)
