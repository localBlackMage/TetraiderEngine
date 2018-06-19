import bpy
me = bpy.context.active_object.data

# tangents have to be pre-calculated
# this will also calculate loop normal
me.calc_tangents()

# loop faces
for face in me.polygons:
    # loop over face loop
    for vert in [me.loops[i] for i in face.loop_indices]:
        tangent = vert.tangent
        normal = vert.normal
        bitangent = vert.bitangent_sign * normal.cross(tangent)