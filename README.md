# harold-the-fc
Harold the flight controller is an arduino based quadcopter flight controller with semi-autonomous capability and a range of safety features.
[WORK IN PROGRESS]

![IMG_20180319_183826](https://user-images.githubusercontent.com/25115039/229012596-b87aaa5e-9903-4fee-949b-45f3c1a45c82.jpg)
![IMG20200412110242](https://user-images.githubusercontent.com/25115039/229012756-1737c33f-b1b6-4833-9c78-0d6e1b692b78.jpg)

## Project story and timeline

### 2017
I started dreaming of this project in 2016 and it really started in 2017, inspired by the many people I saw on social media building quadcopters. I quickly realised that most youtubers in the quadcopter/hobby plane space just buy pre-programmed flight controllers and tune the PID controller values slightly. In retrospect this is a wise approach for anyone without real time signal processing experience.

However at the time I was determined to build the flight controller from scratch


In preparation I read a few books (I'd recommend [this one](https://www.amazon.com/Make-Drones-Teach-Arduino-Fly-ebook/dp/B01MAWF94M/ref=d_pd_sbs_sccl_1_10/134-1054245-3111436?pd_rd_w=gd06T&content-id=amzn1.sym.3676f086-9496-4fd7-8490-77cf7f43f846&pf_rd_p=3676f086-9496-4fd7-8490-77cf7f43f846&pf_rd_r=4N434RZCCXXS6QGERB9J&pd_rd_wg=6sCtU&pd_rd_r=83c685be-baa2-4711-b575-b475ff4d0e68&pd_rd_i=B01MAWF94M&psc=1)) and watched various youtube tutorials (I'd recommend [Joop Brokking's channel](https://www.youtube.com/@Joop_Brokking/videos))

Towards the end of 2017 I put together a list of parts I'd need and hit order.
###  2018/2019
Throughout 2019 I built many small prototypes to be run in test rigs. During this period I learnt _a lot_ and had to completely re-write several times. My initial understanding of the complexity of this undertaking was severely misguided and what I expected to be a couple months work quickly stretched into a year _of just testing_. Still no actual flight.

After a few shaky takeoffs and lots of tuning, Harold finally achieved flight sometime in 2018

Moving forward the goal was to add stabilized mode (or "normal" mode), as so far control was purely "acro" whereby the operator uses the controller to specify the rate of rotation, rather than the angle of pitch/roll/yaw.

So ensued many more months of testing and improvements until eventually I crashed it
