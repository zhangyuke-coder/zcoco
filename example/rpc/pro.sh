#!/bin/bash

protoc ./echo.proto --cpp_out=./
protoc ./args.proto --cpp_out=./