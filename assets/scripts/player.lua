-- Global Variables:
player_velocity = 150

function update()
  -- print("[player.lua] update")  -- TEST
  set_velocity(this, 0, 0)

  x_velocity = 0
  y_velocity = 0

  if is_action_activated("Move Up") then
    print("up")
    y_velocity = y_velocity + -1
  end

  if is_action_activated("Move Down") then
    print("down")
    y_velocity = y_velocity + 1
  end

  if is_action_activated("Move Left") then
    print("left")
    x_velocity = x_velocity + -1
  end

  if is_action_activated("Move Right") then
    print("right")
    x_velocity = x_velocity + 1
  end

  x_velocity = x_velocity * player_velocity
  y_velocity = y_velocity * player_velocity

  set_velocity(this, x_velocity, y_velocity)

end
