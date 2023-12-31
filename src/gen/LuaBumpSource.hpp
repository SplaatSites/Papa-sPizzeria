// Autogenerated file (Do not edit)

#pragma once

constexpr char LUA_BUMP_SOURCE[] =
	"local bump = {\n"
	"  _VERSION     = 'bump v3.1.7',\n"
	"  _URL         = 'https://github.com/kikito/bump.lua',\n"
	"  _DESCRIPTION = 'A collision detection library for Lua',\n"
	"  _LICENSE     = [[\n"
	"    MIT LICENSE\n"
	"\n"
	"    Copyright (c) 2014 Enrique García Cota\n"
	"\n"
	"    Permission is hereby granted, free of charge, to any person obtaining a\n"
	"    copy of this software and associated documentation files (the\n"
	"    \"Software\"), to deal in the Software without restriction, including\n"
	"    without limitation the rights to use, copy, modify, merge, publish,\n"
	"    distribute, sublicense, and/or sell copies of the Software, and to\n"
	"    permit persons to whom the Software is furnished to do so, subject to\n"
	"    the following conditions:\n"
	"\n"
	"    The above copyright notice and this permission notice shall be included\n"
	"    in all copies or substantial portions of the Software.\n"
	"\n"
	"    THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n"
	"    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n"
	"    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n"
	"    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY\n"
	"    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n"
	"    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\n"
	"    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n"
	"  ]]\n"
	"}\n"
	"\n"
	"------------------------------------------\n"
	"-- Auxiliary functions\n"
	"------------------------------------------\n"
	"local DELTA = 1e-10 -- floating-point margin of error\n"
	"\n"
	"local abs, floor, ceil, min, max = math.abs, math.floor, math.ceil, math.min, math.max\n"
	"\n"
	"local function sign(x)\n"
	"  if x > 0 then return 1 end\n"
	"  if x == 0 then return 0 end\n"
	"  return -1\n"
	"end\n"
	"\n"
	"local function nearest(x, a, b)\n"
	"  if abs(a - x) < abs(b - x) then return a else return b end\n"
	"end\n"
	"\n"
	"local function assertType(desiredType, value, name)\n"
	"  if type(value) ~= desiredType then\n"
	"    error(name .. ' must be a ' .. desiredType .. ', but was ' .. tostring(value) .. '(a ' .. type(value) .. ')')\n"
	"  end\n"
	"end\n"
	"\n"
	"local function assertIsPositiveNumber(value, name)\n"
	"  if type(value) ~= 'number' or value <= 0 then\n"
	"    error(name .. ' must be a positive integer, but was ' .. tostring(value) .. '(' .. type(value) .. ')')\n"
	"  end\n"
	"end\n"
	"\n"
	"local function assertIsRect(x,y,w,h)\n"
	"  assertType('number', x, 'x')\n"
	"  assertType('number', y, 'y')\n"
	"  assertIsPositiveNumber(w, 'w')\n"
	"  assertIsPositiveNumber(h, 'h')\n"
	"end\n"
	"\n"
	"local defaultFilter = function()\n"
	"  return 'slide'\n"
	"end\n"
	"\n"
	"------------------------------------------\n"
	"-- Rectangle functions\n"
	"------------------------------------------\n"
	"\n"
	"local function rect_getNearestCorner(x,y,w,h, px, py)\n"
	"  return nearest(px, x, x+w), nearest(py, y, y+h)\n"
	"end\n"
	"\n"
	"-- This is a generalized implementation of the liang-barsky algorithm, which also returns\n"
	"-- the normals of the sides where the segment intersects.\n"
	"-- Returns nil if the segment never touches the rect\n"
	"-- Notice that normals are only guaranteed to be accurate when initially ti1, ti2 == -math.huge, math.huge\n"
	"local function rect_getSegmentIntersectionIndices(x,y,w,h, x1,y1,x2,y2, ti1,ti2)\n"
	"  ti1, ti2 = ti1 or 0, ti2 or 1\n"
	"  local dx, dy = x2-x1, y2-y1\n"
	"  local nx, ny\n"
	"  local nx1, ny1, nx2, ny2 = 0,0,0,0\n"
	"  local p, q, r\n"
	"\n"
	"  for side = 1,4 do\n"
	"    if     side == 1 then nx,ny,p,q = -1,  0, -dx, x1 - x     -- left\n"
	"    elseif side == 2 then nx,ny,p,q =  1,  0,  dx, x + w - x1 -- right\n"
	"    elseif side == 3 then nx,ny,p,q =  0, -1, -dy, y1 - y     -- top\n"
	"    else                  nx,ny,p,q =  0,  1,  dy, y + h - y1 -- bottom\n"
	"    end\n"
	"\n"
	"    if p == 0 then\n"
	"      if q <= 0 then return nil end\n"
	"    else\n"
	"      r = q / p\n"
	"      if p < 0 then\n"
	"        if     r > ti2 then return nil\n"
	"        elseif r > ti1 then ti1,nx1,ny1 = r,nx,ny\n"
	"        end\n"
	"      else -- p > 0\n"
	"        if     r < ti1 then return nil\n"
	"        elseif r < ti2 then ti2,nx2,ny2 = r,nx,ny\n"
	"        end\n"
	"      end\n"
	"    end\n"
	"  end\n"
	"\n"
	"  return ti1,ti2, nx1,ny1, nx2,ny2\n"
	"end\n"
	"\n"
	"-- Calculates the minkowsky difference between 2 rects, which is another rect\n"
	"local function rect_getDiff(x1,y1,w1,h1, x2,y2,w2,h2)\n"
	"  return x2 - x1 - w1,\n"
	"         y2 - y1 - h1,\n"
	"         w1 + w2,\n"
	"         h1 + h2\n"
	"end\n"
	"\n"
	"local function rect_containsPoint(x,y,w,h, px,py)\n"
	"  return px - x > DELTA      and py - y > DELTA and\n"
	"         x + w - px > DELTA  and y + h - py > DELTA\n"
	"end\n"
	"\n"
	"local function rect_isIntersecting(x1,y1,w1,h1, x2,y2,w2,h2)\n"
	"  return x1 < x2+w2 and x2 < x1+w1 and\n"
	"         y1 < y2+h2 and y2 < y1+h1\n"
	"end\n"
	"\n"
	"local function rect_getSquareDistance(x1,y1,w1,h1, x2,y2,w2,h2)\n"
	"  local dx = x1 - x2 + (w1 - w2)/2\n"
	"  local dy = y1 - y2 + (h1 - h2)/2\n"
	"  return dx*dx + dy*dy\n"
	"end\n"
	"\n"
	"local function rect_detectCollision(x1,y1,w1,h1, x2,y2,w2,h2, goalX, goalY)\n"
	"  goalX = goalX or x1\n"
	"  goalY = goalY or y1\n"
	"\n"
	"  local dx, dy      = goalX - x1, goalY - y1\n"
	"  local x,y,w,h     = rect_getDiff(x1,y1,w1,h1, x2,y2,w2,h2)\n"
	"\n"
	"  local overlaps, ti, nx, ny\n"
	"\n"
	"  if rect_containsPoint(x,y,w,h, 0,0) then -- item was intersecting other\n"
	"    local px, py    = rect_getNearestCorner(x,y,w,h, 0, 0)\n"
	"    local wi, hi    = min(w1, abs(px)), min(h1, abs(py)) -- area of intersection\n"
	"    ti              = -wi * hi -- ti is the negative area of intersection\n"
	"    overlaps = true\n"
	"  else\n"
	"    local ti1,ti2,nx1,ny1 = rect_getSegmentIntersectionIndices(x,y,w,h, 0,0,dx,dy, -math.huge, math.huge)\n"
	"\n"
	"    -- item tunnels into other\n"
	"    if ti1\n"
	"    and ti1 < 1\n"
	"    and (abs(ti1 - ti2) >= DELTA) -- special case for rect going through another rect's corner\n"
	"    and (0 < ti1 + DELTA\n"
	"      or 0 == ti1 and ti2 > 0)\n"
	"    then\n"
	"      ti, nx, ny = ti1, nx1, ny1\n"
	"      overlaps   = false\n"
	"    end\n"
	"  end\n"
	"\n"
	"  if not ti then return end\n"
	"\n"
	"  local tx, ty\n"
	"\n"
	"  if overlaps then\n"
	"    if dx == 0 and dy == 0 then\n"
	"      -- intersecting and not moving - use minimum displacement vector\n"
	"      local px, py = rect_getNearestCorner(x,y,w,h, 0,0)\n"
	"      if abs(px) < abs(py) then py = 0 else px = 0 end\n"
	"      nx, ny = sign(px), sign(py)\n"
	"      tx, ty = x1 + px, y1 + py\n"
	"    else\n"
	"      -- intersecting and moving - move in the opposite direction\n"
	"      local ti1, _\n"
	"      ti1,_,nx,ny = rect_getSegmentIntersectionIndices(x,y,w,h, 0,0,dx,dy, -math.huge, 1)\n"
	"      if not ti1 then return end\n"
	"      tx, ty = x1 + dx * ti1, y1 + dy * ti1\n"
	"    end\n"
	"  else -- tunnel\n"
	"    tx, ty = x1 + dx * ti, y1 + dy * ti\n"
	"  end\n"
	"\n"
	"  return {\n"
	"    overlaps  = overlaps,\n"
	"    ti        = ti,\n"
	"    move      = {x = dx, y = dy},\n"
	"    normal    = {x = nx, y = ny},\n"
	"    touch     = {x = tx, y = ty},\n"
	"    itemRect  = {x = x1, y = y1, w = w1, h = h1},\n"
	"    otherRect = {x = x2, y = y2, w = w2, h = h2}\n"
	"  }\n"
	"end\n"
	"\n"
	"------------------------------------------\n"
	"-- Grid functions\n"
	"------------------------------------------\n"
	"\n"
	"local function grid_toWorld(cellSize, cx, cy)\n"
	"  return (cx - 1)*cellSize, (cy-1)*cellSize\n"
	"end\n"
	"\n"
	"local function grid_toCell(cellSize, x, y)\n"
	"  return floor(x / cellSize) + 1, floor(y / cellSize) + 1\n"
	"end\n"
	"\n"
	"-- grid_traverse* functions are based on \"A Fast Voxel Traversal Algorithm for Ray Tracing\",\n"
	"-- by John Amanides and Andrew Woo - http://www.cse.yorku.ca/~amana/research/grid.pdf\n"
	"-- It has been modified to include both cells when the ray \"touches a grid corner\",\n"
	"-- and with a different exit condition\n"
	"\n"
	"local function grid_traverse_initStep(cellSize, ct, t1, t2)\n"
	"  local v = t2 - t1\n"
	"  if     v > 0 then\n"
	"    return  1,  cellSize / v, ((ct + v) * cellSize - t1) / v\n"
	"  elseif v < 0 then\n"
	"    return -1, -cellSize / v, ((ct + v - 1) * cellSize - t1) / v\n"
	"  else\n"
	"    return 0, math.huge, math.huge\n"
	"  end\n"
	"end\n"
	"\n"
	"local function grid_traverse(cellSize, x1,y1,x2,y2, f)\n"
	"  local cx1,cy1        = grid_toCell(cellSize, x1,y1)\n"
	"  local cx2,cy2        = grid_toCell(cellSize, x2,y2)\n"
	"  local stepX, dx, tx  = grid_traverse_initStep(cellSize, cx1, x1, x2)\n"
	"  local stepY, dy, ty  = grid_traverse_initStep(cellSize, cy1, y1, y2)\n"
	"  local cx,cy          = cx1,cy1\n"
	"\n"
	"  f(cx, cy)\n"
	"\n"
	"  -- The default implementation had an infinite loop problem when\n"
	"  -- approaching the last cell in some occassions. We finish iterating\n"
	"  -- when we are *next* to the last cell\n"
	"  while abs(cx - cx2) + abs(cy - cy2) > 1 do\n"
	"    if tx < ty then\n"
	"      tx, cx = tx + dx, cx + stepX\n"
	"      f(cx, cy)\n"
	"    else\n"
	"      -- Addition: include both cells when going through corners\n"
	"      if tx == ty then f(cx + stepX, cy) end\n"
	"      ty, cy = ty + dy, cy + stepY\n"
	"      f(cx, cy)\n"
	"    end\n"
	"  end\n"
	"\n"
	"  -- If we have not arrived to the last cell, use it\n"
	"  if cx ~= cx2 or cy ~= cy2 then f(cx2, cy2) end\n"
	"\n"
	"end\n"
	"\n"
	"local function grid_toCellRect(cellSize, x,y,w,h)\n"
	"  local cx,cy = grid_toCell(cellSize, x, y)\n"
	"  local cr,cb = ceil((x+w) / cellSize), ceil((y+h) / cellSize)\n"
	"  return cx, cy, cr - cx + 1, cb - cy + 1\n"
	"end\n"
	"\n"
	"------------------------------------------\n"
	"-- Responses\n"
	"------------------------------------------\n"
	"\n"
	"local touch = function(world, col, x,y,w,h, goalX, goalY, filter)\n"
	"  return col.touch.x, col.touch.y, {}, 0\n"
	"end\n"
	"\n"
	"local cross = function(world, col, x,y,w,h, goalX, goalY, filter)\n"
	"  local cols, len = world:project(col.item, x,y,w,h, goalX, goalY, filter)\n"
	"  return goalX, goalY, cols, len\n"
	"end\n"
	"\n"
	"local slide = function(world, col, x,y,w,h, goalX, goalY, filter)\n"
	"  goalX = goalX or x\n"
	"  goalY = goalY or y\n"
	"\n"
	"  local tch, move  = col.touch, col.move\n"
	"  if move.x ~= 0 or move.y ~= 0 then\n"
	"    if col.normal.x ~= 0 then\n"
	"      goalX = tch.x\n"
	"    else\n"
	"      goalY = tch.y\n"
	"    end\n"
	"  end\n"
	"\n"
	"  col.slide = {x = goalX, y = goalY}\n"
	"\n"
	"  x,y = tch.x, tch.y\n"
	"  local cols, len  = world:project(col.item, x,y,w,h, goalX, goalY, filter)\n"
	"  return goalX, goalY, cols, len\n"
	"end\n"
	"\n"
	"local bounce = function(world, col, x,y,w,h, goalX, goalY, filter)\n"
	"  goalX = goalX or x\n"
	"  goalY = goalY or y\n"
	"\n"
	"  local tch, move = col.touch, col.move\n"
	"  local tx, ty = tch.x, tch.y\n"
	"\n"
	"  local bx, by = tx, ty\n"
	"\n"
	"  if move.x ~= 0 or move.y ~= 0 then\n"
	"    local bnx, bny = goalX - tx, goalY - ty\n"
	"    if col.normal.x == 0 then bny = -bny else bnx = -bnx end\n"
	"    bx, by = tx + bnx, ty + bny\n"
	"  end\n"
	"\n"
	"  col.bounce   = {x = bx,  y = by}\n"
	"  x,y          = tch.x, tch.y\n"
	"  goalX, goalY = bx, by\n"
	"\n"
	"  local cols, len    = world:project(col.item, x,y,w,h, goalX, goalY, filter)\n"
	"  return goalX, goalY, cols, len\n"
	"end\n"
	"\n"
	"------------------------------------------\n"
	"-- World\n"
	"------------------------------------------\n"
	"\n"
	"local World = {}\n"
	"local World_mt = {__index = World}\n"
	"\n"
	"-- Private functions and methods\n"
	"\n"
	"local function sortByWeight(a,b) return a.weight < b.weight end\n"
	"\n"
	"local function sortByTiAndDistance(a,b)\n"
	"  if a.ti == b.ti then\n"
	"    local ir, ar, br = a.itemRect, a.otherRect, b.otherRect\n"
	"    local ad = rect_getSquareDistance(ir.x,ir.y,ir.w,ir.h, ar.x,ar.y,ar.w,ar.h)\n"
	"    local bd = rect_getSquareDistance(ir.x,ir.y,ir.w,ir.h, br.x,br.y,br.w,br.h)\n"
	"    return ad < bd\n"
	"  end\n"
	"  return a.ti < b.ti\n"
	"end\n"
	"\n"
	"local function addItemToCell(self, item, cx, cy)\n"
	"  self.rows[cy] = self.rows[cy] or setmetatable({}, {__mode = 'v'})\n"
	"  local row = self.rows[cy]\n"
	"  row[cx] = row[cx] or {itemCount = 0, x = cx, y = cy, items = setmetatable({}, {__mode = 'k'})}\n"
	"  local cell = row[cx]\n"
	"  self.nonEmptyCells[cell] = true\n"
	"  if not cell.items[item] then\n"
	"    cell.items[item] = true\n"
	"    cell.itemCount = cell.itemCount + 1\n"
	"  end\n"
	"end\n"
	"\n"
	"local function removeItemFromCell(self, item, cx, cy)\n"
	"  local row = self.rows[cy]\n"
	"  if not row or not row[cx] or not row[cx].items[item] then return false end\n"
	"\n"
	"  local cell = row[cx]\n"
	"  cell.items[item] = nil\n"
	"  cell.itemCount = cell.itemCount - 1\n"
	"  if cell.itemCount == 0 then\n"
	"    self.nonEmptyCells[cell] = nil\n"
	"  end\n"
	"  return true\n"
	"end\n"
	"\n"
	"local function getDictItemsInCellRect(self, cl,ct,cw,ch)\n"
	"  local items_dict = {}\n"
	"  for cy=ct,ct+ch-1 do\n"
	"    local row = self.rows[cy]\n"
	"    if row then\n"
	"      for cx=cl,cl+cw-1 do\n"
	"        local cell = row[cx]\n"
	"        if cell and cell.itemCount > 0 then -- no cell.itemCount > 1 because tunneling\n"
	"          for item,_ in pairs(cell.items) do\n"
	"            items_dict[item] = true\n"
	"          end\n"
	"        end\n"
	"      end\n"
	"    end\n"
	"  end\n"
	"\n"
	"  return items_dict\n"
	"end\n"
	"\n"
	"local function getCellsTouchedBySegment(self, x1,y1,x2,y2)\n"
	"\n"
	"  local cells, cellsLen, visited = {}, 0, {}\n"
	"\n"
	"  grid_traverse(self.cellSize, x1,y1,x2,y2, function(cx, cy)\n"
	"    local row  = self.rows[cy]\n"
	"    if not row then return end\n"
	"    local cell = row[cx]\n"
	"    if not cell or visited[cell] then return end\n"
	"\n"
	"    visited[cell] = true\n"
	"    cellsLen = cellsLen + 1\n"
	"    cells[cellsLen] = cell\n"
	"  end)\n"
	"\n"
	"  return cells, cellsLen\n"
	"end\n"
	"\n"
	"local function getInfoAboutItemsTouchedBySegment(self, x1,y1, x2,y2, filter)\n"
	"  local cells, len = getCellsTouchedBySegment(self, x1,y1,x2,y2)\n"
	"  local cell, rect, l,t,w,h, ti1,ti2, tii0,tii1\n"
	"  local visited, itemInfo, itemInfoLen = {},{},0\n"
	"  for i=1,len do\n"
	"    cell = cells[i]\n"
	"    for item in pairs(cell.items) do\n"
	"      if not visited[item] then\n"
	"        visited[item]  = true\n"
	"        if (not filter or filter(item)) then\n"
	"          rect           = self.rects[item]\n"
	"          l,t,w,h        = rect.x,rect.y,rect.w,rect.h\n"
	"\n"
	"          ti1,ti2 = rect_getSegmentIntersectionIndices(l,t,w,h, x1,y1, x2,y2, 0, 1)\n"
	"          if ti1 and ((0 < ti1 and ti1 < 1) or (0 < ti2 and ti2 < 1)) then\n"
	"            -- the sorting is according to the t of an infinite line, not the segment\n"
	"            tii0,tii1    = rect_getSegmentIntersectionIndices(l,t,w,h, x1,y1, x2,y2, -math.huge, math.huge)\n"
	"            itemInfoLen  = itemInfoLen + 1\n"
	"            itemInfo[itemInfoLen] = {item = item, ti1 = ti1, ti2 = ti2, weight = min(tii0,tii1)}\n"
	"          end\n"
	"        end\n"
	"      end\n"
	"    end\n"
	"  end\n"
	"  table.sort(itemInfo, sortByWeight)\n"
	"  return itemInfo, itemInfoLen\n"
	"end\n"
	"\n"
	"local function getResponseByName(self, name)\n"
	"  local response = self.responses[name]\n"
	"  if not response then\n"
	"    error(('Unknown collision type: %s (%s)'):format(name, type(name)))\n"
	"  end\n"
	"  return response\n"
	"end\n"
	"\n"
	"\n"
	"-- Misc Public Methods\n"
	"\n"
	"function World:addResponse(name, response)\n"
	"  self.responses[name] = response\n"
	"end\n"
	"\n"
	"function World:project(item, x,y,w,h, goalX, goalY, filter)\n"
	"  assertIsRect(x,y,w,h)\n"
	"\n"
	"  goalX = goalX or x\n"
	"  goalY = goalY or y\n"
	"  filter  = filter  or defaultFilter\n"
	"\n"
	"  local collisions, len = {}, 0\n"
	"\n"
	"  local visited = {}\n"
	"  if item ~= nil then visited[item] = true end\n"
	"\n"
	"  -- This could probably be done with less cells using a polygon raster over the cells instead of a\n"
	"  -- bounding rect of the whole movement. Conditional to building a queryPolygon method\n"
	"  local tl, tt = min(goalX, x),       min(goalY, y)\n"
	"  local tr, tb = max(goalX + w, x+w), max(goalY + h, y+h)\n"
	"  local tw, th = tr-tl, tb-tt\n"
	"\n"
	"  local cl,ct,cw,ch = grid_toCellRect(self.cellSize, tl,tt,tw,th)\n"
	"\n"
	"  local dictItemsInCellRect = getDictItemsInCellRect(self, cl,ct,cw,ch)\n"
	"\n"
	"  for other,_ in pairs(dictItemsInCellRect) do\n"
	"    if not visited[other] then\n"
	"      visited[other] = true\n"
	"\n"
	"      local responseName = filter(item, other)\n"
	"      if responseName then\n"
	"        local ox,oy,ow,oh   = self:getRect(other)\n"
	"        local col           = rect_detectCollision(x,y,w,h, ox,oy,ow,oh, goalX, goalY)\n"
	"\n"
	"        if col then\n"
	"          col.other    = other\n"
	"          col.item     = item\n"
	"          col.type     = responseName\n"
	"\n"
	"          len = len + 1\n"
	"          collisions[len] = col\n"
	"        end\n"
	"      end\n"
	"    end\n"
	"  end\n"
	"\n"
	"  table.sort(collisions, sortByTiAndDistance)\n"
	"\n"
	"  return collisions, len\n"
	"end\n"
	"\n"
	"function World:countCells()\n"
	"  local count = 0\n"
	"  for _,row in pairs(self.rows) do\n"
	"    for _,_ in pairs(row) do\n"
	"      count = count + 1\n"
	"    end\n"
	"  end\n"
	"  return count\n"
	"end\n"
	"\n"
	"function World:hasItem(item)\n"
	"  return not not self.rects[item]\n"
	"end\n"
	"\n"
	"function World:getItems()\n"
	"  local items, len = {}, 0\n"
	"  for item,_ in pairs(self.rects) do\n"
	"    len = len + 1\n"
	"    items[len] = item\n"
	"  end\n"
	"  return items, len\n"
	"end\n"
	"\n"
	"function World:countItems()\n"
	"  local len = 0\n"
	"  for _ in pairs(self.rects) do len = len + 1 end\n"
	"  return len\n"
	"end\n"
	"\n"
	"function World:getRect(item)\n"
	"  local rect = self.rects[item]\n"
	"  if not rect then\n"
	"    error('Item ' .. tostring(item) .. ' must be added to the world before getting its rect. Use world:add(item, x,y,w,h) to add it first.')\n"
	"  end\n"
	"  return rect.x, rect.y, rect.w, rect.h\n"
	"end\n"
	"\n"
	"function World:toWorld(cx, cy)\n"
	"  return grid_toWorld(self.cellSize, cx, cy)\n"
	"end\n"
	"\n"
	"function World:toCell(x,y)\n"
	"  return grid_toCell(self.cellSize, x, y)\n"
	"end\n"
	"\n"
	"\n"
	"--- Query methods\n"
	"\n"
	"function World:queryRect(x,y,w,h, filter)\n"
	"\n"
	"  assertIsRect(x,y,w,h)\n"
	"\n"
	"  local cl,ct,cw,ch = grid_toCellRect(self.cellSize, x,y,w,h)\n"
	"  local dictItemsInCellRect = getDictItemsInCellRect(self, cl,ct,cw,ch)\n"
	"\n"
	"  local items, len = {}, 0\n"
	"\n"
	"  local rect\n"
	"  for item,_ in pairs(dictItemsInCellRect) do\n"
	"    rect = self.rects[item]\n"
	"    if (not filter or filter(item))\n"
	"    and rect_isIntersecting(x,y,w,h, rect.x, rect.y, rect.w, rect.h)\n"
	"    then\n"
	"      len = len + 1\n"
	"      items[len] = item\n"
	"    end\n"
	"  end\n"
	"\n"
	"  return items, len\n"
	"end\n"
	"\n"
	"function World:queryPoint(x,y, filter)\n"
	"  local cx,cy = self:toCell(x,y)\n"
	"  local dictItemsInCellRect = getDictItemsInCellRect(self, cx,cy,1,1)\n"
	"\n"
	"  local items, len = {}, 0\n"
	"\n"
	"  local rect\n"
	"  for item,_ in pairs(dictItemsInCellRect) do\n"
	"    rect = self.rects[item]\n"
	"    if (not filter or filter(item))\n"
	"    and rect_containsPoint(rect.x, rect.y, rect.w, rect.h, x, y)\n"
	"    then\n"
	"      len = len + 1\n"
	"      items[len] = item\n"
	"    end\n"
	"  end\n"
	"\n"
	"  return items, len\n"
	"end\n"
	"\n"
	"function World:querySegment(x1, y1, x2, y2, filter)\n"
	"  local itemInfo, len = getInfoAboutItemsTouchedBySegment(self, x1, y1, x2, y2, filter)\n"
	"  local items = {}\n"
	"  for i=1, len do\n"
	"    items[i] = itemInfo[i].item\n"
	"  end\n"
	"  return items, len\n"
	"end\n"
	"\n"
	"function World:querySegmentWithCoords(x1, y1, x2, y2, filter)\n"
	"  local itemInfo, len = getInfoAboutItemsTouchedBySegment(self, x1, y1, x2, y2, filter)\n"
	"  local dx, dy        = x2-x1, y2-y1\n"
	"  local info, ti1, ti2\n"
	"  for i=1, len do\n"
	"    info  = itemInfo[i]\n"
	"    ti1   = info.ti1\n"
	"    ti2   = info.ti2\n"
	"\n"
	"    info.weight  = nil\n"
	"    info.x1      = x1 + dx * ti1\n"
	"    info.y1      = y1 + dy * ti1\n"
	"    info.x2      = x1 + dx * ti2\n"
	"    info.y2      = y1 + dy * ti2\n"
	"  end\n"
	"  return itemInfo, len\n"
	"end\n"
	"\n"
	"\n"
	"--- Main methods\n"
	"\n"
	"function World:add(item, x,y,w,h)\n"
	"  local rect = self.rects[item]\n"
	"  if rect then\n"
	"    error('Item ' .. tostring(item) .. ' added to the world twice.')\n"
	"  end\n"
	"  assertIsRect(x,y,w,h)\n"
	"\n"
	"  self.rects[item] = {x=x,y=y,w=w,h=h}\n"
	"\n"
	"  local cl,ct,cw,ch = grid_toCellRect(self.cellSize, x,y,w,h)\n"
	"  for cy = ct, ct+ch-1 do\n"
	"    for cx = cl, cl+cw-1 do\n"
	"      addItemToCell(self, item, cx, cy)\n"
	"    end\n"
	"  end\n"
	"\n"
	"  return item\n"
	"end\n"
	"\n"
	"function World:remove(item)\n"
	"  local x,y,w,h = self:getRect(item)\n"
	"\n"
	"  self.rects[item] = nil\n"
	"  local cl,ct,cw,ch = grid_toCellRect(self.cellSize, x,y,w,h)\n"
	"  for cy = ct, ct+ch-1 do\n"
	"    for cx = cl, cl+cw-1 do\n"
	"      removeItemFromCell(self, item, cx, cy)\n"
	"    end\n"
	"  end\n"
	"end\n"
	"\n"
	"function World:update(item, x2,y2,w2,h2)\n"
	"  local x1,y1,w1,h1 = self:getRect(item)\n"
	"  w2,h2 = w2 or w1, h2 or h1\n"
	"  assertIsRect(x2,y2,w2,h2)\n"
	"\n"
	"  if x1 ~= x2 or y1 ~= y2 or w1 ~= w2 or h1 ~= h2 then\n"
	"\n"
	"    local cellSize = self.cellSize\n"
	"    local cl1,ct1,cw1,ch1 = grid_toCellRect(cellSize, x1,y1,w1,h1)\n"
	"    local cl2,ct2,cw2,ch2 = grid_toCellRect(cellSize, x2,y2,w2,h2)\n"
	"\n"
	"    if cl1 ~= cl2 or ct1 ~= ct2 or cw1 ~= cw2 or ch1 ~= ch2 then\n"
	"\n"
	"      local cr1, cb1 = cl1+cw1-1, ct1+ch1-1\n"
	"      local cr2, cb2 = cl2+cw2-1, ct2+ch2-1\n"
	"      local cyOut\n"
	"\n"
	"      for cy = ct1, cb1 do\n"
	"        cyOut = cy < ct2 or cy > cb2\n"
	"        for cx = cl1, cr1 do\n"
	"          if cyOut or cx < cl2 or cx > cr2 then\n"
	"            removeItemFromCell(self, item, cx, cy)\n"
	"          end\n"
	"        end\n"
	"      end\n"
	"\n"
	"      for cy = ct2, cb2 do\n"
	"        cyOut = cy < ct1 or cy > cb1\n"
	"        for cx = cl2, cr2 do\n"
	"          if cyOut or cx < cl1 or cx > cr1 then\n"
	"            addItemToCell(self, item, cx, cy)\n"
	"          end\n"
	"        end\n"
	"      end\n"
	"\n"
	"    end\n"
	"\n"
	"    local rect = self.rects[item]\n"
	"    rect.x, rect.y, rect.w, rect.h = x2,y2,w2,h2\n"
	"\n"
	"  end\n"
	"end\n"
	"\n"
	"function World:move(item, goalX, goalY, filter)\n"
	"  local actualX, actualY, cols, len = self:check(item, goalX, goalY, filter)\n"
	"\n"
	"  self:update(item, actualX, actualY)\n"
	"\n"
	"  return actualX, actualY, cols, len\n"
	"end\n"
	"\n"
	"function World:check(item, goalX, goalY, filter)\n"
	"  filter = filter or defaultFilter\n"
	"\n"
	"  local visited = {[item] = true}\n"
	"  local visitedFilter = function(itm, other)\n"
	"    if visited[other] then return false end\n"
	"    return filter(itm, other)\n"
	"  end\n"
	"\n"
	"  local cols, len = {}, 0\n"
	"\n"
	"  local x,y,w,h = self:getRect(item)\n"
	"\n"
	"  local projected_cols, projected_len = self:project(item, x,y,w,h, goalX,goalY, visitedFilter)\n"
	"\n"
	"  while projected_len > 0 do\n"
	"    local col = projected_cols[1]\n"
	"    len       = len + 1\n"
	"    cols[len] = col\n"
	"\n"
	"    visited[col.other] = true\n"
	"\n"
	"    local response = getResponseByName(self, col.type)\n"
	"\n"
	"    goalX, goalY, projected_cols, projected_len = response(\n"
	"      self,\n"
	"      col,\n"
	"      x, y, w, h,\n"
	"      goalX, goalY,\n"
	"      visitedFilter\n"
	"    )\n"
	"  end\n"
	"\n"
	"  return goalX, goalY, cols, len\n"
	"end\n"
	"\n"
	"\n"
	"-- Public library functions\n"
	"\n"
	"bump.newWorld = function(cellSize)\n"
	"  cellSize = cellSize or 64\n"
	"  assertIsPositiveNumber(cellSize, 'cellSize')\n"
	"  local world = setmetatable({\n"
	"    cellSize       = cellSize,\n"
	"    rects          = {},\n"
	"    rows           = {},\n"
	"    nonEmptyCells  = {},\n"
	"    responses = {}\n"
	"  }, World_mt)\n"
	"\n"
	"  world:addResponse('touch', touch)\n"
	"  world:addResponse('cross', cross)\n"
	"  world:addResponse('slide', slide)\n"
	"  world:addResponse('bounce', bounce)\n"
	"\n"
	"  return world\n"
	"end\n"
	"\n"
	"bump.rect = {\n"
	"  getNearestCorner              = rect_getNearestCorner,\n"
	"  getSegmentIntersectionIndices = rect_getSegmentIntersectionIndices,\n"
	"  getDiff                       = rect_getDiff,\n"
	"  containsPoint                 = rect_containsPoint,\n"
	"  isIntersecting                = rect_isIntersecting,\n"
	"  getSquareDistance             = rect_getSquareDistance,\n"
	"  detectCollision               = rect_detectCollision\n"
	"}\n"
	"\n"
	"bump.responses = {\n"
	"  touch  = touch,\n"
	"  cross  = cross,\n"
	"  slide  = slide,\n"
	"  bounce = bounce\n"
	"}\n"
	"\n"
	"return bump\n"
	"";
