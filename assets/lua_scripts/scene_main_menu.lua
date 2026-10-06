scene = {
  -- Images and sprites table:
  sprites = {},

  -- Fonts Table:
  fonts = {
    {id = "FONT TEST SIZE 24", file_path = "./assets/fonts/valu_old_caps.ttf", size = 24},
    {id = "Taipei24", file_path = "./assets/fonts/Taipei24.ttf", size = 24}
  },

  -- Actions and Keys Table (renamed to 'keys' to match C++):
  keys = {
    {name = "Move Up", key = 119},
    {name = "Move Left", key = 97},
    {name = "Move Down", key = 115},
    {name = "Move Right", key = 100}
  },

  -- Actions and Mouse Buttons Table:
  buttons = {
    {name = "mouse_left_button", code = 1}
  },

  -- Entities Table:
  entities = {
    {
      -- Entity 2 - Font 1:
      components = {
        clickable = {},

        text = {
          text = "Score: 69",
          id = "FONT TEST SIZE 24",
          r = 150,
          g = 0,
          b = 150,
          a = 255
        },

        transform = {
          position = {x = 500.0, y = 50.0},
          scale = {x = 1.0, y = 1.0},
          rotation = 0.0
        }
      }
    },

    {
      -- Entity 3 - Font 2:
      components = {
        clickable = {},

        script = {
          path = "./assets/lua_scripts/main_menu_button_01.lua"
        },

        text = {
          text = "TAIPEI",
          id = "Taipei24",
          r = 200,
          g = 0,
          b = 200,
          a = 100
        },

        transform = {
          position = {x = 200.0, y = 50.0},
          scale = {x = 1.0, y = 1.0},
          rotation = 0.0
        }
      }
    }
  }
}
