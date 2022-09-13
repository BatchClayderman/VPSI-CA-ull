import os
from re import findall
from pandas import DataFrame as DF
os.chdir(os.path.abspath(os.path.dirname(__file__)))#解析进入程序所在目录
EXIT_SUCCESS = 0
EXIT_FAILURE = 1
EOF = (-1)
csvResult = "result.csv"


def getValue(content) -> list:
	lRet = [None] * 6
	for line in content.split("\n"):
		# ALL
		pattern_time = findall("Time:.*\\ =\\ .*ms", line)
		if pattern_time:
			lRet[0] = findall("=\\ .*ms", pattern_time[0])[0][2:-2]
			continue
		pattern_corresponding = findall("sizeof\\(\\*\\)\\ =\\ \\d+\\ KB\\ \\(\\*\\)", line)
		if pattern_corresponding:
			lRet[1] = findall("\\d+", pattern_corresponding[0])[0]
			continue
		
		# R
		pattern_time = findall("Timeof\\(R\\)\\ =\\ .*\\ =\\ .*ms", line)
		if pattern_time:
			lRet[2] = pattern_time[0].split(" = ")[-1][:-2]
			continue
		pattern_corresponding = findall("sizeof\\(R\\)\\ =\\ \\d+\\ KB", line)
		if pattern_corresponding:
			lRet[3] = findall("\\d+", pattern_corresponding[0])[0]
			continue
		
		# S
		pattern_time = findall("Timeof\\(S\\)\\ =\\ .*\\ =\\ .*ms", line)
		if pattern_time:
			lRet[4] = pattern_time[0].split(" = ")[-1][:-2]
			continue
		pattern_corresponding = findall("sizeof\\(S\\)\\ =\\ \\d+\\ KB", line)
		if pattern_corresponding:
			lRet[5] = findall("\\d+", pattern_corresponding[0])[0]
			continue
	return lRet

def getlist(folder = ".", encoding = "utf-8") -> list:
	lists = []
	for txt in os.listdir(folder):
		if txt.lower().endswith(".txt"):
			try:
				with open(txt, "r", encoding = encoding) as f:
					content = f.read()
				print([txt] + getValue(content))
				lists.append(os.path.splitext(txt)[0].split("_")[1:] + getValue(content))
			except Exception as e:
				print(e)
	return lists			

def main() -> int:
	lists = getlist()
	try:
		DF(lists, columns = ["Alg", "m", "Time_ALL (ms)", "Space_ALL (KB)", "Time_R (ms)", "Space_R (KB)", "Time_S (ms)", "Space_S (KB)"]).to_csv(csvResult, index = False)
		input()
		return EXIT_SUCCESS
	except Exception as e:
		print(e)
		input()
		return EXIT_FAILURE



if __name__ == "__main__":
	exit(main())