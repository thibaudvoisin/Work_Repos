FROM python:latest

WORKDIR /worker

COPY . .

RUN pip install -r requirements.txt

EXPOSE 9000

CMD ["flask", "run", "--host=0.0.0.0","--port=9000"]