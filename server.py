#importing required modules
import os
from wsgiref.simple_server import make_server
from flask import Flask, jsonify, make_response, render_template, request, request_started
from dotenv import load_dotenv
import mysql.connector

load_dotenv()
app = Flask(__name__)
db = mysql.connector.connect(host=os.get_env('DB_host'),user=os.getenv("DB_user"),password=os.getenv("DB_password"),database=os.getenv("DB_name"))
cursor = db.cursor()

cursor.execute("CREATE TABLE IF NOT EXISTS(username varchar(30), password char(64), phoneno int(12), interest1 int(1), interest2 int(2), interest3 int(3), interest4 int(4), interest5 int(5), interest6 int(6), interest7 int(7), interest8 int(8), interest9 int(9), interest10 int(10), interest11 int(11), interest12 int(12), interest13 int(13), interest14 int(14), interest15 int(15), interest16 int(16), interest17 int(17), interest18 int(18), interest19 int(19), interest20 int(20), interest21 int(21), interest22 int(22), interest23 int(23), interest24 int(24), interest25 int(25), interest26 int(26), interest27 int(27), interest28 int(28), interest29 int(29), interest30 int(30), interest31 int(31), interest32 int(32), interest33 int(33), interest34 int(34), interest35 int(35), interest36 int(36), interest37 int(37), interest38 int(38), interest39 int(39), interest40 int(40), interest41 int(41), interest42 int(42), interest43 int(43), interest44 int(44), interest45 int(45), interest46 int(46), interest47 int(47), interest48 int(48), interest49 int(49), interest50 int(50), interest51 int(51), interest52 int(52), interest53 int(53), interest54 int(54), interest55 int(55), interest56 int(56), interest57 int(57), interest58 int(58), interest59 int(59), interest60 int(60), interest61 int(61), interest62 int(62), interest63 int(63), interest64 int(64), interest65 int(65), interest66 int(66), interest67 int(67), interest68 int(68), interest69 int(69), interest70 int(70), interest71 int(71), interest72 int(72), interest73 int(73), interest74 int(74), interest75 int(75), secret_question1 varchar(20), secret_answer1 varchar(10), secret_question2 varchar(20), secret_answer2 varchar(10), secret_question3 varchar(20), secret_answer3 varchar(10)")


@app.route('/')
def home():
    return render_template('index.html')

@app.route('/login', methods=["POST", "GET"])
def login_validation():
    if request.method == 'GET':
        if session.get('session_user') != "":
            return redirect(url_for('dashboard'))
        return render_template('login.html')
    if request.method == 'POST':
        username = str(request.form.get('username'))
        password = hashlib.sha256(str(request.form.get('password')).encode()).hexdigest()
        cursor.execute("SELECT password FROM users WHERE username=\"" + username + "\"")
        returndata = db.fetchall()
        if (hashlibsha256(returndata[0].encode()).hexdigest() == password):
            session['session_user'] = username
        else:
            flash("incorrect credentials")


@app.route('/dashboard')
def dashboard():
    session_user = session.get('session_user')
    if user_id:
        # User is logged in, show dashboard
        return "Welcome to your dashboard, user {}".format(session_user)
    else:
        # User is not logged in, redirect to login page
        return redirect(url_for('login'))

@app.route('/logout')
def logout():
    # Clear the session data and redirect to the login page
    session.clear()
    return redirect(url_for('login'))

        


if __name__ == '__main__':
    app.run(port=int(os.getenv('APP_PORT')))
    #app.run(port=1234)
