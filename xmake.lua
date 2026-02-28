set_project("DualPadXInput")
set_version("1.0.0")
set_languages("c++23")

local skyrim_root = "G:/g/SkyrimSE"

target("xinput1_3")
    set_kind("shared")
    set_runtimes("MT")  -- 静态链接 C++ 运行时
    
    add_files("src/xinput_wrapper.cpp")
    add_files("xinput1_3.def")
    
    set_targetdir(skyrim_root)
    
    after_build(function (target)
        print("Deployed to: %s", target:targetfile())
    end)