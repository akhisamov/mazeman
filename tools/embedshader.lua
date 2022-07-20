tabs = "    "

function assert_arguments()
    print("Wrong Arguments:\n\tembedshader.lua [-o|--output] {output_file} -- {input_files}")
    os.exit(false)
end

function get_output_file(arg)
    for k, v in ipairs(arg) do
        if v == "-o" or v == "--output" then
            return arg[k + 1]
        elseif v == "--" and k == 2 then
            return arg[k - 1]
        end
    end
    assert_arguments()
end

function get_input_files(arg)
    for k, v in ipairs(arg) do
        if v == "--" then
            local input_files = {}
            local j = 1
            for i = k + 1, #(arg), 1 do
                input_files[j] = arg[i]
                j = j + 1
            end
            return input_files
        end
    end
    assert_arguments()
end

function get_output_content(input_files)
    local output_content =
        "// ----------------------\n"..
        "// THIS IS GENERATED FILE\n"..
        "// ----------------------\n"..
        "#pragma once\n\n"..
        "#include <string_view>\n\n"..
        "namespace shaders\n"..
        "{\n"..
        "%s\n"..
        "}"

    local shaders = {}
    for k,v in ipairs(input_files) do
        local name = v:match("^.+/(.+)$")
        name = string.gsub(name, "%.", "_")
        local content = ""
        local f = io.open(v, "r")
        for line in f:lines() do
            local line_content = line:gsub(tabs, "")
            content = content .. tabs .. tabs .. string.format("\"%s\\n\"\n", line_content)
        end
        content = content:sub(1, -2)
        f:close()
        shaders[name] = content
    end

    local variables = ""
    local i = 0
    for k,v in pairs(shaders) do
        local var = tabs .. "inline constexpr std::string_view %s =\n%s;"
        variables = variables .. var:format(k, v)
        if not (i + 1 == #(input_files)) then
            variables = variables .. "\n\n"
        end
        i = i + 1
    end

    return output_content:format(variables)
end

local output_file = get_output_file(arg)
local input_files = get_input_files(arg)
local output_content = get_output_content(input_files)

local f = io.open(output_file, "w+")
f:write(output_content)
f:close()
