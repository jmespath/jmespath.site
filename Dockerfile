FROM python:2

WORKDIR /usr/src/app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

RUN make html
WORKDIR /usr/src/app/docs/_build/html

EXPOSE 8000

CMD [ "python", "-m", "SimpleHTTPServer" ]
