tabs = "    "

function get_output_content(input_file, namespace)
    local output_content =
        "// ----------------------\n"..
        "// THIS IS GENERATED FILE\n"..
        "// ----------------------\n"..
        "#pragma once\n\n"..
        "#include <string_view>\n\n"..
        "namespace %s\n"..
        "{\n"..
        "    inline constexpr std::string_view %s =\n%s;\n"..
        "}"
    local input_name = input_file:match("^.+/(.+)$"):match("(.+)%..+")
    local input_content = ""
    local f = io.open(input_file, "r")
    for line in f:lines() do
        local line_content = line:gsub(tabs, "")
        input_content = input_content .. tabs .. tabs .. string.format("\"%s\\n\"\n", line_content)
    end
    input_content = input_content:sub(1, -2)
    f:close()

    return output_content:format(namespace,input_name, input_content)
end

if not #arg == 3 then
    print("Wrong Arguments:\n\tembed.lua {input_file} {output_file}")
    os.exit(false)
end

local input_file = arg[1]
local output_file = arg[2]
local namespace = output_file:match("^(.+)/"):match("^.+/(.+)$")
local output_content = get_output_content(input_file, namespace)

local f = io.open(output_file, "w+")
f:write(output_content)
f:close()
