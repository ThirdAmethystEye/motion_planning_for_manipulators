def collision_section(joints, obstacles):
    for i in range(joints):
        for j in range(obstacles):
            f.write(' ' * 8 + f'<pair geom1="geom edge {i}" geom2="geom obstacle {j}"/>\n')
    for i in range(joints):
        for j in range(joints):
            if j - i > 1:
                f.write(' ' * 8 + f'<pair geom1="geom edge {i}" geom2="geom edge {j}"/>\n')

def manipulator_autogen(joints, size, pos):
    output = ''
    output_suffix = []
    output_prefix = ''
    current_space = '        '
    for i in range(joints):
        if i == 0:
            euler = '\"0 90 0\"'
        else:
            euler = '\"0 0 0\"'
        output_prefix += f'{current_space}<body name=\"edge {i}\" pos={pos[f"edge_{i}"]} euler={euler}>\n'
        
        output_prefix += f'{current_space + "    "}<joint name=\"joint {i}\" type=\"hinge\" axis=\"-1 0 0\" pos={pos["joints"]}/>\n'
        output_prefix += f'{current_space + "    "}<geom name=\"geom edge {i}\" type=\"cylinder\" size={size["cylinder"]} material=\"blue\"/>\n'
        if i == joints - 1:
            output_prefix += f'{current_space + "    "}<site name=\"tip\" size={size["tip"]} pos={pos["tip"]}/>\n'
        output_suffix.append(f'{current_space}</body>\n')
        current_space += '    '
    output += output_prefix + ''.join(output_suffix[-1:-len(output_suffix)-1:-1]) + '\n'
    current_space = '        '
    output_prefix = ''
    output_suffix = []
    for i in range(joints):
        if i == 0:
            euler = '\"0 90 0\"'
        else:
            euler = '\"0 0 0\"'
        output_prefix += f'{current_space}<body name=\"edge {i} shade\" pos={pos[f"edge_{i}"]} euler={euler}>\n'
        output_prefix += f'{current_space + "    "}<joint name=\"joint {i} shade\" type=\"hinge\" axis=\"-1 0 0\" pos={pos["joints"]}/>\n'
        output_prefix += f'{current_space + "    "}<geom name=\"geom edge {i} shade\" type=\"cylinder\" size={size["cylinder"]} material=\"green\"/>\n'
        output_suffix.append(f'{current_space}</body>\n')
        current_space += '    '
    output += output_prefix + ''.join(output_suffix[-1:-len(output_suffix)-1:-1])
    return output

joints = 2
obstacles = 6

file_path_to_edges = 'tools/edges.txt'
file_path_to_obstacles = 'tools/obstacles.txt'

with open(f'tools/{joints}-dof_{obstacles}-obs_manipulator.xml', "w+") as f:
    f.write(open('tools/const/header.txt', 'r').read())
    f.write('\n')
    f.write(manipulator_autogen(joints, {"cylinder":'\"0.5 0 0.1\"', \
    "tip":'\"0.1\"'}, {"edge_0":'\"0.5 0 0.1\"', "edge_1":'\"0 0 1\"', "joints":'\"0 0 -0.5\"', "tip":'\"0 0 0.4\"'}))
    f.write('\n')
    f.write(open(file_path_to_obstacles, 'r').read())
    f.write('\n')
    f.write('    </worldbody>\n\n')
    f.write('    <contact>\n')
    collision_section(joints, obstacles)    
    f.write('    </contact>\n')
    f.write(open('tools/const/footer.txt', 'r').read())
