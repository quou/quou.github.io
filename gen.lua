html_start = [[
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
<title>George Lamb</title>
<style>
body {
	margin: 40px auto;
	max-width: 650px;
	line-height: 1.6;
	font-size: 18px;
	color: #444;
}

body {
	margin: auto;
	width: 50%;
	padding: 0 10px;
}

@media screen and (max-width:900px) {
	body {
		margin: auto;
		width: 90%;
		padding: 0 10px;
	}
}

@media screen and (min-width:2000px) {
	body {
		margin: 40px auto;
		max-width: 1000px;
		line-height: 1.6;
		font-size: 18px;
		color: #444;
	}
}

div.gallery img {
  width: 100%;
  height: 150px;
}

div.desc {
  padding: 15px;
  height: 100px;
  text-align: center;
}

* {
  box-sizing: border-box;
}

.responsive {
  padding: 0 6px;
  float: left;
  width: 24.99999%;
}

@media only screen and (max-width: 700px) {
  .responsive {
    width: 49.99999%;
    margin: 6px 0;
  }
}

@media only screen and (max-width: 500px) {
  .responsive {
    width: 100%;
  }
}

.thumb {
	object-fit: cover;
}

.clearfix:after {
  content: "";
  display: table;
  clear: both;
}

code {
	font-weight: bold;
	color: #000000;
	font-size: 14px;
}

pre {
	line-height: 1;
}

img {
	max-width: 100%;
	border-radius: 5px;
}

a {
	color: #444;
	background-color: white;
}

a:hover {
	color: white;
	background-color: #444;
}

@media (prefers-color-scheme: dark) {
	body {
		background-color: #1a1a1a;
		color: #d6d6d6;
	}

	a {
		color: #d6d6d6;
		background-color: #1a1a1a;
	}

	a:hover {
		color: #1a1a1a;
		background-color: #d6d6d6;
	}

	code {
		color: #bababa;
	}
}
</style>
</head>
<body>
]]

html_end = [[
</body>
</html>
]]

function split(s, sep)
	local fields = {}

	local sep = sep or " "
	local pattern = string.format("([^%s]+)", sep)
	string.gsub(s, pattern, function(c) fields[#fields + 1] = c end)

	return fields
end

function parse(input, output)
	local lines = {}
	for l in io.lines(input) do
		lines[#lines + 1] = l
	end

	if #lines < 1 then return end

	local f = io.open(output, "w");

	f:write(html_start)

	local in_p = false
	local in_codeblock = false
	local in_code = false
	local in_link_url = false
	local in_link_text = false
	local link_is_image = false
	local in_bold = false
	local in_italic = false
	local in_ul = false

	local link_url = ""
	local link_text = ""

	local last = ""
	for i, l in ipairs(lines) do
		if not in_codeblock then
			if string.sub(l, 1, 3) == "```" then
				in_codeblock = true
				f:write("<pre><code>")
				goto newline
			elseif string.sub(l, 1, 3) == " - " then
				if not in_ul then
					f:write("<ul>")
				end
				f:write("<li>")
				l = string.sub(l, 4, -1)
				in_ul = true
			end

			local in_head = false
			local hc = 1
			while string.sub(l, hc, hc) == "#" do
				in_head = true
				hc = hc + 1
			end

			if in_head then
				l = string.sub(l, hc, -1)
				f:write("<h" .. hc - 1  .. ">")
			elseif not in_ul and (last == "" or string.sub(last, 1, 1) == "#" or
					(last == "\n" and l == "\n")) then
				in_p = true
				f:write("<p>")
			end

			if string.sub(l, 1, 2) == "--" then
				f:write("<hr/>")
			else
				local last_c = ""
				for ii = 1, #l do
					local c = string.sub(l, ii, ii)

					if c == "`" then
						if not in_code then
							f:write("<code>")
							in_code = true
						else
							f:write("</code>")
							in_code = false
						end
					end

					if c == "[" and not in_code then
						link_is_image = last_c == "!"

						in_link_text = true
						link_url = ""
						link_text = ""
					elseif c == "]" and in_link_text then
					elseif c == "(" and in_link_text then
						in_link_text = false
						in_link_url = true
					elseif c == ")" and in_link_url then
						in_link_url = false
						if link_is_image then
							f:write("<img src=\"" .. link_url .. "\" alt=\"" .. link_text .. "\"/>")
						else
							f:write("<a href=\"" .. link_url .. "\">" .. link_text .. "</a>")
						end
					elseif in_link_text then
						link_text = link_text .. c
					elseif in_link_url then
						link_url = link_url .. c
					elseif c == "*" and not in_code then
						if last_c == "*" then
							if not in_bold then
								f:write("<strong>")
							else
								f:write("</strong>")
							end
							in_bold = not in_bold
						elseif string.sub(l, ii + 1, ii + 1) ~= "*" then
							if not in_italic then
								f:write("<i>")
							else
								f:write("</i>")
							end
							in_italic = not in_italic
						end
					elseif c ~= "`" and not (c == "!" and string.sub(l, ii + 1, ii + 1) == "[") then
						f:write(c)
					end

					last_c = c
				end
			end

			if in_ul then
				if lines[i + 1] == nil or #lines[i + 1] <= 1 then
					f:write("</li></ul>")
					in_ul = false
				elseif string.sub(lines[i + 1], 1, 3) == " - " then
					f:write("</li>")
				end
			end

			if in_head then
				f:write("</h" .. hc - 1  .. ">")
			elseif in_p then
				if l ~= "\n" and last == "\n" then
					f:write(" ")
				end

				if lines[i + 1] == nil or #lines[i + 1] <= 1 then
					in_p = false
					f:write("</p>")	
				end
			end
		else
			if string.sub(l, 1, 3) == "```" then
				in_codeblock = false
				f:write("</code></pre>")
			else
				l = string.gsub(l, "<", "&lt;")
				l = string.gsub(l, ">", "&gt;")
				f:write(l)
			end
		end

		::newline::
		f:write("\n")

		last = l
	end

	f:write(html_end)
end

for l in io.lines("list") do
	local pair = split(l, " ")

	parse(pair[1], pair[2])
end
