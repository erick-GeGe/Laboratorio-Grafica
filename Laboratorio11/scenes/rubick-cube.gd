extends Spatial


# Declare member variables here. Examples:
var angle = 0.0
var velocity = 300.0
var init_rotation_x = []
var node
var rotating = false
var calculed_cubes = false
var v_aux = false
var r_rotate = false
var l_rotate = false
var u_rotate = false
var d_rotate = false
var f_rotate = false
var b_rotate = false
var r_rotate_ = false
var l_rotate_ = false
var u_rotate_ = false
var d_rotate_ = false
var f_rotate_ = false
var b_rotate_ = false

# Called when the node enters the scene tree for the first time.
func _ready():
	
	for i in range(1,get_child_count()):
		init_rotation_x.push_back(Vector3.ZERO)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	_input_rotation()
	if rotating:
		
		angle += (velocity*delta)
		angle = min(angle, 90.0)
		
		# CALCULATE CUBES TO ROTATE
		if not calculed_cubes:
			_calculate_cubes()
			calculed_cubes = true
		
		# ROTATING
		if r_rotate or l_rotate:
			node.rotation.x = deg2rad(angle)
		if u_rotate or d_rotate:
			node.rotation.y = deg2rad(angle)
		if f_rotate or b_rotate:
			node.rotation.z = deg2rad(angle)
			
		if r_rotate_ or l_rotate_:
			node.rotation.x = -deg2rad(angle)
		if u_rotate_ or d_rotate_:
			node.rotation.y = -deg2rad(angle)
		if f_rotate_ or b_rotate_:
			node.rotation.z = -deg2rad(angle)
			
#		 STOP ANIMATION
		if angle == 90.0:
			for i in range(node.get_child_count()-1,-1 ,-1):
				var hijo = node.get_child(i)
				hijo.transform = hijo.global_transform
				node.remove_child(hijo)
				add_child(hijo)
			_reset_values()
			

		
func _reset_values():
	rotating = false
	calculed_cubes = false
	
	r_rotate = false
	l_rotate = false
	u_rotate = false
	d_rotate = false
	f_rotate = false
	b_rotate = false
	r_rotate_ = false
	l_rotate_ = false
	u_rotate_ = false
	d_rotate_ = false
	f_rotate_ = false
	b_rotate_ = false
	
	get_parent().remove_child(node)
	angle = 0.0
		
func _calculate_cubes():
	node = Spatial.new()
	get_parent().add_child(node)
	for i in range(get_child_count()-1,0 ,-1):
		var hijo = get_child(i)
		if r_rotate or r_rotate_:
			if(hijo.global_transform.origin.x > 0.9):
				remove_child(hijo)
				node.add_child(hijo)
		elif l_rotate or l_rotate_:
			if(hijo.global_transform.origin.x < -0.9): 
				remove_child(hijo)
				node.add_child(hijo)
		elif u_rotate or u_rotate_:
			if(hijo.global_transform.origin.y > 0.9): 
				remove_child(hijo)
				node.add_child(hijo)
		elif d_rotate or d_rotate_:
			if(hijo.global_transform.origin.y < -0.9): 
				remove_child(hijo)
				node.add_child(hijo)
		elif f_rotate or f_rotate_:
			if(hijo.global_transform.origin.z > 0.9): 
				remove_child(hijo)
				node.add_child(hijo)
		elif b_rotate or b_rotate_:
			if(hijo.global_transform.origin.z < -0.9): 
				remove_child(hijo)
				node.add_child(hijo)


func _input_rotation():
	if Input.is_action_just_pressed("R"):
		if not rotating:
			rotating = true
			r_rotate = true
	if Input.is_action_just_pressed("L"):
		if not rotating:
			rotating = true
			l_rotate = true
	if Input.is_action_just_pressed("U"):
		if not rotating:
			rotating = true
			u_rotate = true
	if Input.is_action_just_pressed("D"):
		if not rotating:
			rotating = true
			d_rotate = true
	if Input.is_action_just_pressed("F"):
		if not rotating:
			rotating = true
			f_rotate = true
	if Input.is_action_just_pressed("B"):
		if not rotating:
			rotating = true
			b_rotate = true
			
	if Input.is_action_just_pressed("R_"):
		if not rotating:
			rotating = true
			r_rotate_ = true
	if Input.is_action_just_pressed("L_"):
		if not rotating:
			rotating = true
			l_rotate_ = true
	if Input.is_action_just_pressed("U_"):
		if not rotating:
			rotating = true
			u_rotate_ = true
	if Input.is_action_just_pressed("D_"):
		if not rotating:
			rotating = true
			d_rotate_ = true
	if Input.is_action_just_pressed("F_"):
		if not rotating:
			rotating = true
			f_rotate_ = true
	if Input.is_action_just_pressed("B_"):
		if not rotating:
			rotating = true
			b_rotate_ = true
