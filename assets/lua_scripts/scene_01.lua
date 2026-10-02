scene = {
  -- Images and sprites table:
  sprites = {
    {id = "Player", file_path = "./assets/Player.png"},
    {id = "enemy_1", file_path = "./assets/enemy_1.png"}
  },

  -- Fonts Table:
  fonts = {
    {id = "FONT TEST SIZE 24", file_path = "./assets/fonts/valu_old_caps.ttf", size = 24}
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
      -- Entity 1 - Player:
      components = {
        animation = {},

        circle_collider = {
          radius = 8,
          width = 16,
          height = 16
        },

        rigid_body = {
          velocity = {x = 0, y = 0}
        },

        script = {
          path = "./assets/lua_scripts/player.lua"
        },

        sprite = {
          asset_id = "Player",
          width = 16,
          height = 16,
          source_rectangle = {x = 0, y = 0}
        },

        transform = {
          position = {x = 400, y = 300},
          scale = {x = 2.0, y = 2.0},
          rotation = 0.0
        }
      }
    },
    {
      -- Entity 2 - Font:
      components = {
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
          rotation = 0.0,
        }
      }
    }
  }
}
