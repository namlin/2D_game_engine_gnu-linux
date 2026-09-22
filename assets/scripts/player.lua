-- Global Variables:
player_velocity = 150

-- Normalization factor for diagonal movement: 1 / sqrt(2) ≈ 0.70710678
diagonal_factor = 1 / math.sqrt(2)

function update()
  set_velocity(this, 0, 0)

  x_velocity = 0
  y_velocity = 0

  if is_action_activated("Move Up") then
    y_velocity = y_velocity - 1
  end

  if is_action_activated("Move Down") then
    y_velocity = y_velocity + 1
  end

  if is_action_activated("Move Left") then
    x_velocity = x_velocity - 1
  end

  if is_action_activated("Move Right") then
    x_velocity = x_velocity + 1
  end

  -- Reassign variables when moving diagonally:
  if x_velocity ~= 0 and y_velocity ~= 0 then
    x_velocity = x_velocity * diagonal_factor * player_velocity
    y_velocity = y_velocity * diagonal_factor * player_velocity
  else
    x_velocity = x_velocity * player_velocity
    y_velocity = y_velocity * player_velocity
  end

  set_velocity(this, x_velocity, y_velocity)
end
