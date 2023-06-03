#!/bin/sh
background='false'
while getopts 'b' flag; do
  case "${flag}" in
    b) background='true' ;;
    *) echo "Usage: $0 [-b]" >&2
       exit 1 ;;
  esac
done

main_loop() {
while true; do
    python - << EOF
import os
import sys
import time
from datetime import datetime
from telebot import TeleBot

bot = TeleBot('TOKEN')
bot_start_time = time.time()

def is_message_new(message):
    message_time = datetime.fromtimestamp(message.date)
    return message_time.timestamp() > bot_start_time

@bot.message_handler(commands=['help'])
def help_command(message):
    if is_message_new(message):
        bot.reply_to(message, '/saldo /today and number')

@bot.message_handler(commands=['saldo'])
def saldo_command(message):
    if is_message_new(message):
        result = os.popen('./saldo').read()
        bot.reply_to(message, result)

@bot.message_handler(commands=['today'])
def today_command(message):
    if is_message_new(message):
        result = os.popen('./saldo --today').read()
        bot.reply_to(message, result)

@bot.message_handler(func=lambda message: True)
def number_added_command(message):
    if is_message_new(message):
        try:
            number = message.text
            result = os.popen('./saldo ' + number).read()
            bot.reply_to(message, 'Number added. ' + result)
        except ValueError:
            bot.reply_to(message, 'Wrong command!')

bot.polling()
EOF
    sleep 10
done
}

if [ "$background" = 'true' ]; then
  nohup sh -c 'main_loop' &
else
  main_loop
fi
