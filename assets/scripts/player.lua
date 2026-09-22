function update()
  print("[player.lua] update")  -- TEST

  if is_action_activated("Move Up") then
    print("up")
  end

  if is_action_activated("Move Down") then
    print("down")
  end

  if is_action_activated("Move Left") then
    print("left")
  end

  if is_action_activated("Move Right") then
    print("right")
  end

end
