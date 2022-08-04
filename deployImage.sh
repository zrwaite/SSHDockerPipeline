docker container ls  | grep $1 | awk '{print $1}' | xargs docker stop

docker system prune -f

docker run -d -p $2:$2 $1

docker image prune -f -a
